# IoT-Enabled-Smart-Helmet-for-Miners
This repository contains the Arduino and HTML code for an IoT-enabled smart helmet designed to enhance miner safety. The smart helmet is equipped with various sensors to monitor environmental conditions and ensure the safety of miners by providing real-time data and alerts.

Setup Instructions
Blynk Configuration:

Create a Blynk project and note the BLYNK_AUTH_TOKEN.
Replace the BLYNK_AUTH_TOKEN in the code with your token.
Circuit Assembly:

Connect the GPS module to pins defined (rxGPS and txGPS).
Set up DHT11 and gas sensors according to the pin configuration in the code.
Attach the buzzer to the specified pin (D7).
Software Setup:

Install all required libraries in the Arduino IDE.
Update the ssid and password variables with your WiFi credentials.
Upload the .ino file to your ESP8266 module.
Both the .ino file and the index.h file should be placed in the same directory. This is crucial for the Arduino IDE to correctly compile and upload the sketch to your ESP8266 board, as the sketch includes references to index.h.

Running the Web Server:

The web server starts automatically after the ESP8266 boots up.
Access the web server via the ESP8266's IP address to view GPS data readings.
Usage
The smart helmet will begin monitoring once powered on and connected to the WiFi network.
Real-time data is sent to the Blynk app where environmental readings and alerts can be monitored.
The helmet will alert the miner with a buzzer sound if hazardous conditions are detected or if the miner exits a predefined geofenced area.

Contributions
Contributions are welcome! Please fork the repository and submit a pull request with your suggested changes.
