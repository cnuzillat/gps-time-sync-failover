# GPS Time Sync Failover

An embedded time synchronization system designed for a Raspberry Pi that acquires precise UTC time from a GPS receiver and automatically switches to a backup time source during GPS outages. The project features real-time monitoring, event logging, and a web-based dashboard to visualize system status and uptime. This project demonstrates reliable timekeeping for aerospace and defense applications where accurate timing is critical.

---

## Features

- **GPS-based Time Synchronization**: Retrieves precise UTC time from a GPS receiver (VK-172 USB dongle).  
- **Failover to RTC**: Uses a DS3231 Real-Time Clock module as a backup when GPS signal is lost.  
- **Real-time Monitoring**: Displays system status, current coordinates, and time accuracy.  
- **Event Logging**: Logs time sync events, GPS fixes, and failover actions for analysis.  
- **Web-based Dashboard (future)**: Planned interface to visualize uptime and system metrics.

---

## Project Objectives

1. Implement a robust GPS time acquisition system in C, interfacing with `gpsd`.
2. Understand and handle NMEA sentence parsing and GPS data structures. 
3. Implement a failover timekeeping system using a DS3231 RTC module.
4. Demonstrate embedded development practices for aerospace-grade reliability and precision.

---

## Relevance to Aerospace

Accurate and redundant timekeeping is foundational for aerospace systems, including:

- Avionics and flight control synchronization
- Satellite communication timing
- Logging and telemetry alignment
- Fault-tolerant mission systems

This project demonstrates how precise UTC synchronization can be achieved using open-source software and commodity hardware in embedded environments.

---

## Bill of Materials

| Component | Purpose | Link |
|-----------|---------|------|
| Raspberry Pi 4 Model B | Main compute board | [Amazon](https://www.amazon.com/Raspberry-Pi-4-Model-4GB/dp/B07TD42S27/) |
| MicroSD Card (32GB) | Storage for Raspberry Pi OS | [Amazon](https://www.amazon.com/Raspberry-Pi-Official-MicroSD-32GB/dp/B09V4SK9VJ/) |
| Geekworm Power Supply, 20W 5V 4A | Reliable power for Raspberry Pi | [Amazon](https://www.amazon.com/Geekworm-Raspberry-Adapter-Charger-Support/dp/B09J856PND) |
| VK-172 USB GPS Receiver | Primary GPS time source | [Amazon](https://www.amazon.com/WWZMDiB-VK-172-USB-Dongle-Receiver/dp/B0BVBLXVLQ) |
| CANADUINO DS3231 RTC Module | Backup time source | [Amazon](https://www.amazon.com/CANADUINO%C2%AE-DS3231-Module-Interface-Battery/dp/B07BCPRH6F) |
| Raspberry Pi Case | Enclosure and protection | [Official Case Link](https://www.raspberrypi.com/products/raspberry-pi-4-case/) |

---

## Progress

Progress is tracked in [`progress.md`](progress.md)

As of the most recent update:
- Successfully installed and configured `gpsd` and `gpsd-clients`.
- Verified detection of VK-172 GPS dongle via `/dev/ttyACM0`.
- Observed satellite lock and fix data using `cgps -s`.
- Developed an initial C program (`gps_time.c`) that:
  - Connects to `gpsd`
  - Reads GPS fix data
  - Outputs current UTC time once per second
- Testing showed stable 3D DGPS fix with 5–6 satellites in use.

---

## Getting Started

1. Prepare Raaspberry Pi
```bash
sudo apt update && sudo apt upgrade
sudo apt install gpsd gpsd-clients
```
2. Connect GPS Device
   - Plug the VK-172 GPS dongle into a USB port.
   - Verify detection:
  ```bash
lsusb
ls /dev/ttyACM*
```   
3. Start gpsd
```bash
sudo systemctl stop gpsd.socket gpsd
sudo gpsd /dev/ttyACM0 -F /var/run/gpsd.sock
```
4. Run the C Program
```bash
gcc gps_time.c -o gps_time -lgps
./gps_time
```

---

## Next Steps

- Improve GPS parsing and validation logic in C.
- Add support for DS3231 RTC as a backup time source.
- Implement logging of time synchronization events.
- Capture and document GPS output screenshots and hardware setup images.
- Develop the planned web dashboard for real-time status visualization.
