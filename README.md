# IoT-Enabled-Smart-Helmet-for-Miners
This repository contains the Arduino and HTML code for an IoT-enabled smart helmet designed to enhance miner safety. The smart helmet is equipped with various sensors to monitor environmental conditions and ensure the safety of miners by providing real-time data and alerts.

## Features
1. Environmental Monitoring: Measures temperature, humidity, and detects hazardous gases (e.g., methane).
2. Location Tracking: Uses GPS for real-time location tracking and geofencing.
3. Alert System: Integrates with Blynk for mobile notifications and in-helmet alerts using a buzzer.
4. Web Server: An onboard web server displays GPS data and system status.

## Hardware Requirements
1. ESP8266 WiFi Module
2. GPS Module (using SoftwareSerial)
3. DHT11 Temperature and Humidity Sensor
4. MQ-135 and MQ-2 Gas Sensors
5. Buzzer for Alerts
6. General electronic components (resistors, wires, breadboard, etc.)

## Software Requirements
1. Arduino IDE for compiling and uploading the firmware.
2. Blynk app setup with the corresponding widgets for monitoring.
3. Libraries: TinyGPS++, SoftwareSerial, ESP8266WiFi, WiFiClient, ESP8266WebServer, BlynkSimpleEsp8266, DHT.

## Setup Instructions

**Blynk Configuration:**

1. Create a Blynk project and note the BLYNK_AUTH_TOKEN.
2. Replace the BLYNK_AUTH_TOKEN in the code with your token.

**Circuit Assembly:**

1. Connect the GPS module to pins defined (rxGPS and txGPS).
2. Set up DHT11 and gas sensors according to the pin configuration in the code.
3. Attach the buzzer to the specified pin (D7).

**Software Setup:**

1. Install all required libraries in the Arduino IDE.
2. Update the ssid and password variables with your WiFi credentials.
3. Upload the .ino file to your ESP8266 module.
> Both the .ino file and the index.h file should be placed in the same directory. This is crucial for the Arduino IDE to correctly compile and upload the sketch to your ESP8266 board, as the sketch includes references to index.h.

**Running the Web Server:**

The web server starts automatically after the ESP8266 boots up.
Access the web server via the ESP8266's IP address to view GPS data readings.

## Usage
The smart helmet will begin monitoring once powered on and connected to the WiFi network.
Real-time data is sent to the Blynk app where environmental readings and alerts can be monitored.
The helmet will alert the miner with a buzzer sound if hazardous conditions are detected or if the miner exits a predefined geofenced area.

## Contributions
Contributions are welcome! Please fork the repository and submit a pull request with your suggested changes.
