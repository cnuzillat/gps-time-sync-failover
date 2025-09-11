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

## Goals

1. Build a reliable embedded timekeeping system using Raspberry Pi and C programming.  
2. Understand and parse GPS NMEA sentences for accurate time acquisition.  
3. Implement failover mechanisms with an RTC module.  
4. Develop modular, testable code suitable for aerospace-grade reliability.  

---

## Relevance to Aerospace

Accurate and resilient timekeeping is crucial for aerospace applications, including:

- Aircraft navigation and avionics systems  
- Communication synchronization for distributed systems  
- Mission-critical logging and event correlation  

This project demonstrates the integration of hardware and software to achieve robust timing under real-world conditions.

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

Progress is tracked in [`progress.md`](progress.md) including setup, GPS testing, and initial system verification.  

---

## Getting Started

1. Flash Raspberry Pi OS Lite (64-bit) onto the microSD card.  
2. Connect GPS receiver and RTC module to the Pi.  
3. SSH into the Raspberry Pi and update packages:  
```bash
sudo apt update && sudo apt upgrade
