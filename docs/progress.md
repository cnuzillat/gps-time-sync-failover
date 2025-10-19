# Project Progress Log

This document tracks the ongoing progress of the Raspberry Pi GPS Timekeeping Project.  

---

## Repository Setup
- Created GitHub repository with `README.md` describing:
  - Project title, goals, and relevance to aerospace applications.  
  - Bill of materials (with purchase links).  
- Added `.gitignore` to exclude build artifacts, logs, and editor-specific files.  

---

## Hardware Selection & Ordering
- Chose **Raspberry Pi 4 Model B** as the main compute board.  
- Ordered components:  
  - [Geekworm Power Supply for Raspberry Pi 4](https://www.amazon.com/Geekworm-Raspberry-Adapter-Charger-Support/dp/B09J856PND)  
  - [CANADUINO DS3231 RTC Module](https://www.amazon.com/CANADUINO%C2%AE-DS3231-Module-Interface-Battery/dp/B07BCPRH6F)  
  - [VK-172 USB GPS Receiver](https://www.amazon.com/WWZMDiB-VK-172-USB-Dongle-Receiver/dp/B0BVBLXVLQ)  
  - Raspberry Pi 4 board, official microSD card (32 GB), and case.  

---

## Software Preparation
- Installed **Raspberry Pi Imager** on laptop.  
- Downloaded **Raspberry Pi OS Lite (64-bit)**.  
- Reviewed how to flash an SD card and enable **SSH** and **Wi-Fi** using `ssh` and `wpa_supplicant.conf` files.  
- Noted that actual flashing requires microSD card and reader.  

---

## GPS Research
- Reviewed **gpsd** documentation to understand Linux GPS handling.  
- Learned about key **NMEA sentences**:  
  - `$GPRMC` → Minimum data set (time, date, fix, speed).  
  - `$GPGGA` → Fix data (position, altitude, satellites).  
- Added reference notes (`gps_notes.md`) to repository.  

---

## Raspberry Pi Setup
- Flashed microSD card with **Raspberry Pi OS Lite**.  
- Inserted card into Raspberry Pi and powered on.  
- Connected to Wi-Fi and successfully accessed Pi via **SSH**.  
- Performed initial system updates:  
  ```bash
  sudo apt update && sudo apt upgrade

---

## GPS Hardware and Testing
- Installed GPS software tools for device communication and monitoring:
  ```bash
  sudo apt install gpsd gpsd-clients
- Connected VK-172 USB GPS dongle and verified detection:
  ```bash
  lsusb
  ls /dev/ttyACM
  ```
  - Confirmed device appears as `/dev/ttyACM0`.
- Started gpsd service and pointed it to the GPS device:
  ```bash
  sudo systemctl stop gpsd.socket gpsd
  sudo gpsd /dev/ttyACM0 -F /var/run/gpsd.sock
- Explored cgps -s to understand its output and structure:
  - Learned how `cgps` displays satellite status, fix type, latitude, longitude, altitude, and error metrics.
  - Observed how the dashboard updates as satellites are acquired.
- Confirmed GPS operation:
  - Satellites Seen: 17
  - Satellites Used in Fix: 6
  - Status: 3D DGPS FIX, indicating successful position fix.
- Observations:
  - Indoor/window placement results in slower initial satellite fix; outdoor placement will improve acquisition time.
  - GPS coordinates and error metrics stabilize after a few minutes.

---

## C Programming and GPSD Integration
- Transitioned from Python to C programming for direct GPSD communication.
- Created and edited the first C source file `gps_time.c` using VS Code (Remote SSH to the Pi).
- Installed necessary libraries for GPS access:
  ```bash
  sudo apt install libgps-dev
- Wrote and compiled the C program using:
  ```bash
  gcc gps_time.c -o gps_time -lgps
- Program functionality:
  - Connects to the GPSD daemon running on the Pi.
  - Continuously checks for valid GPS data.
  - Prints current UTC time from the GPS receiver once a valid fix is obtained.
  - Displays “Waiting for GPS fix...” while the receiver acquires satellites.
- Verified successful compilation and execution on the Pi.
- Practiced safely stopping programs (`Ctrl + C`) and shutting down the Pi using:
  ```bash
  sudo shutdown now
- Troubleshooting insights:
  - Initial compilation errors were caused by missing GPS library includes (resolved by installing `libgps-dev`).
  - First fix delays were due to limited satellite visibility indoors, placement near a window or outdoors helps acquire lock faster.

---

# Configure NTP Backup (Chrony)
- Installed Chrony with:
```bash
sudo apt install chrony
```
- Edited `/etc/chrony/chrony.conf` to use the NTP pool:
```bash
pool pool.ntp.org iburst
```
- Restarted Chrony and verified synchronization:
```bash
sudo systemctl restart chronyd
chronyc tracking
```
- Confirmed successful sync with output:
```bash
Reference ID    : A29FC801 (time.cloudflare.com)
Stratum         : 4
System time     : 0.000000067 seconds fast of NTP time
Leap status     : Normal
```
-The system is now accurately maintaining time using NTP as a backup source when GPS is unavailable.

---

## NTP Time Retrieval in C
- Implemented C-based NTP client to serve as the software backup time source when GPS data is unavailable.
- Created `ntp_time.c`, which:
  - Opens a UDP socket to `time.nist.gov` (port 123).
  - Sends a 48-byte NTP request packet.
  - Receives the response and extracts the transmit timestamp (bytes 40-43)
  - Converts the NTP time (seconds since 1900) to standard Unix epoch (seconds since 1970).
  - Prints UTC time in human-readable format.
- Commands used:
```bash
gcc ntp_time.c -o ntp_time
./ntp_time
```
- Verified successful NTP communication and accurate UTC timestamp output.
- This step establishes the software failover mechanism for time synchronization, complementing the GPS hardware source.

---

## GPS to NTP Failover Implementation
Objective:
  - Implement automatic failover detection between GPS and NTP time sources.
Tasks Completed:
  - Create new file: `gps_ntp_failover.c`
  - Added logic to monitor GPS signal activity
  - Implemented 5-second timeout: If no GPS data is received for more than 5 seconds, the program automatically switches to NTP time.
  - Logs all failover events to the console for transparency.
  - Automatically reverts to GPS once a valid GPS fix is restored.
Key Features:
  - Real-time UTC time display.
  - Seamless transition between GPS and NTP.
  - Clear console logs for debugging and monitoring.

Compilation Command:
```bash
gcc gps_ntp_failover.c -o gps_ntp_failover -lgps
```
Run Command:
```bash
./gps_ntp_failover
```
Example Output:
```pgsql
[INFO] GPS and NTP failover system started.
GPS Time (UTC): 18:25:40 | Lat: 43.678945 | Lon: -70.125612
[LOG] GPS signal lost for >5 seconds, switching to NTP time.
NTP Time (UTC): 18:25:47
[LOG] GPS fix restored, switching back to GPS.
GPS Time (UTC): 18:25:55 | Lat: 43.678945 | Lon: -70.125612
```
Notes:
  - Ensures continous time availabilty even if GPS temporarily fails.
  - Can be extended later to log data to a file or integrate with Chrony for hybrid time correction.
