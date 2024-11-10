# Arduino Project with Sensor Data & LED Control

This project uses an **Arduino Uno R4 WiFi**, a **DHT11 sensor** for temperature and humidity readings, and an LED that can be controlled remotely. The data is sent to **ThingSpeak** for real-time monitoring, and a simple web interface allows users to control the LED.

## Features
- Real-time temperature and humidity monitoring
- Remote LED control via a web interface
- Integration with ThingSpeak for data storage and visualization

## Hardware Requirements
- **Arduino Uno R4 WiFi**
- **DHT11 Sensor** (for temperature and humidity)
- **LED** (for control)
- **Wi-Fi** (Arduino Uno R4 WiFi has built-in Wi-Fi support)

## Software Requirements
- Arduino IDE
- ThingSpeak account for data storage
- Web browser to access the web interface

## How It Works
- Arduino Uno R4 WiFi collects sensor data and sends it to ThingSpeak.
- The web interface fetches and displays the data and allows the LED to be toggled on/off.
- The page updates every 30 seconds with the latest data.

## Setup Instructions

### Clone the Repository
```bash
git clone https://github.com/your-username/arduino-project.git

