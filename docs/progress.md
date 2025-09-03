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
