# ESP32 FM Radio

### What is the purpose of the project
This project aims to use an ESP32 board to listen to FM radio stations and play music via the board's integrated Bluetooth module. It also features a sleek GUI on an OLED display!

### The idea
I’ve seen many radios at rural market stalls and wanted to create my own to see if I could make something better.

### Why I think this project is useful
I want to learn and create something I can use at home. Hopefully, others will also find it useful for their own projects.

### Functions
1. Core functionality
- FM Radio player
- Play audio through Bluetooth speakers
- Use as an external Bluetooth speaker
- Support for internet radio stations
- Audio Equalizer

2. GUI
- Search for FM channels and save them (also supports RDS - internet connection required)
- Visual volume control
- Signal strength meter (for Radio, Wi-Fi and Bluetooth)
- Clock and Alarm
- Weather forecast
- Timer and Stopwatch
- Audio Visualizer (Winamp good old days)
- Wi-Fi settings and network status
- Bluetooth settings and status
- Audio Equalizer settings
- System debug information (RAM, CPU, etc.)

## Block Diagram
TODO
## Hardware Design
- 1x Espressiff ESP-WROOM-32 with ESP32 Chip
- 1x TEA5767 FM Radio Module
- 1x SSD1306 OLED Screen Board
- Multiple cables
- 1x Button (play/pause)
- 1x Joystick (volume control and menu navigation)

## Software Design
For this project I used PlatformIO IDE.
<br>
The code is written in C++ and uses the Arduino framework.
### Libraries
- TEA5767 for FM radio
- Adafruit SSD1306 for OLED display control
- Adafruit GFX for graphics
- LVGL for GUI design (follow this [instructions](https://docs.lvgl.io/8.3/get-started/platforms/arduino.html#configure-lvgl))
- ESP32-A2DP for Bluetooth audio
- arduino-audio-tools for audio processing
- ArduinoJson for parsing JSON data from the internet
- ESPASyncWebServer for handling web requests