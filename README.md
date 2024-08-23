# Smart Irrigation System

## What does it do?
Monitors soil moisture and temperature in real time using ESP32 and DHT22 sensor.
Automatically turns the water pump on when soil is too dry and off when moisture is restored.
Sends Telegram alerts whenever irrigation starts or stops with timestamp and readings.

## Why I built this
Water wastage in agriculture is a huge problem in India.
I built this as a low cost automatic irrigation solution that waters plants only when needed,
saving water and removing the need for manual monitoring.

## Hardware used
- ESP32
- DHT22 Temperature and Humidity Sensor
- Soil Moisture Sensor
- Water Pump
- Relay Module

## Features
- Real time soil moisture and temperature monitoring
- Automatic pump control based on moisture threshold
- Telegram alert with timestamp when irrigation starts and stops
- Pump cycle counter
- Dry and wet threshold configurable in code

## How to run
1. Open main.cpp in Arduino IDE
2. Connect sensors as per pin definitions in code
3. Enter your WiFi name and password
4. Enter your Telegram username and CallMeBot API key
5. Upload to ESP32
6. Open Serial Monitor at 115200 baud

## Built by
Gauresh Pathak — Electronics and Telecommunication Engineering Student, Mumbai
