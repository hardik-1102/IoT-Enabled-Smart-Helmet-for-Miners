// Link your Blynk authorization token to your project

#define BLYNK_TEMPLATE_ID "TMPL3FTvHOEgF"
#define BLYNK_TEMPLATE_NAME "Smart Helmet"
#define BLYNK_AUTH_TOKEN "6wM3rhyJ-EYvcCJdO-GCDKcapm0gS-jr"

#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>
#include "index.h"

char auth[] = BLYNK_AUTH_TOKEN;

#define M_PI 3.14159265358979323846264338327950288
#define rxGPS 4
#define txGPS 5

// Coordinates to create the geofence

const double fences[1][10][2] = {{{17.529188, 78.361845},
                                  {17.529840, 78.361919},
                                  {17.529934, 78.362197},
                                  {17.530624, 78.362507},
                                  {17.530832, 78.363043},
                                  {17.530999, 78.363451},
                                  {17.530924, 78.363976},
                                  {17.529248, 78.363288},
                                  {17.529101, 78.362858},
                                  {17.529040, 78.362489},}
};

/*Variables to store AP credentials*/
String ssid = "AndroidAP";
String password = "87654321";

//change the wifi ID PASS
char ssid1[] = "AndroidAP";
char password1[] = "87654321";

int WiFiConnectMode = 1; /* 0: smart config; 1: hard code*/

double latitude, longitude;
int sat;
String date;
char lati[12];
char longi[12];
int targetStatus;
int fence;
char cumulativeAngle[12];
int deviceStatus = 0;

int buzzer  = D7;

int mq135 = 14;
int mq2 = 12;
int mq135_data=0;
int mq2_data=0;

DHT dht(D4, DHT11);

SoftwareSerial gpsSerial(rxGPS, txGPS);
TinyGPSPlus gps;
ESP8266WebServer gpsServer(80);

void connectWifi();
void updateLatLon();
void pip();
void handleRoot();
void fenceSelect();
void gps_data();

int mq135_read(){
  digitalWrite(mq135, HIGH);
  digitalWrite(mq2, LOW);
  return analogRead(0);
}

int mq2_read(){
  digitalWrite(mq135, LOW);
  digitalWrite(mq2, HIGH);
  return analogRead(0);
}


void setup(){
    Serial.begin(9600);
    gpsSerial.begin(9600);
    connectWifi();

    gpsServer.on("/", handleRoot);
    gpsServer.on("/status", fenceSelect);
    gpsServer.on("/values", gps_data);
    gpsServer.begin();
    pinMode(mq135, OUTPUT);
    pinMode(mq2, OUTPUT);
    // pinMode(buzzer, OUTPUT);
    //Start the Blynk connection
    Blynk.begin(auth,ssid1,password1);

    //Initialize the DHT sensor
    dht.begin();
}


 
void loop(){
    
  
    while (gpsSerial.available()){
        
        deviceStatus = 1;
        updateLatLon();
        pip();        
        gpsServer.handleClient();
        
    }
    
    double temp = dht.readTemperature();
    double hum  = dht.readHumidity();

    //change the threshold according to your usage
    if(temp>35){
      tone(buzzer, 1000, 1000);
      Blynk.logEvent("temperature_alert", "Temperature above 30 degrees.");
    }else
    noTone(buzzer);
    
  //Print the sensor data on the serial monitor
    Serial.print("Temperature: ");
    Serial.print(temp);
    Serial.print(" °C, Humidity: ");
    Serial.print(hum);
    Serial.println("% ");
    mq135_data = mq135_read();
    int mq135_calibrated = mq135_data*2+23;
    if(mq135_calibrated > 500){
      Blynk.logEvent("aqi_alert","AQI Level above 500ppm.");
    }
    delay(500);
    mq2_data = mq2_read();
    int mq2_calibrated = mq2_data + 17;
    if(mq2_calibrated > 500){
      Blynk.logEvent("methane_gas_alert","High Concentration in Methane Gas.");
    }
    delay(500);
    Serial.print("MQ135: ");

    
    Serial.print(mq135_calibrated);
    
    
    Serial.print("\nMQ2: ");
    Serial.print(mq2_calibrated);
    
    Blynk.virtualWrite(V0, temp);
    Blynk.virtualWrite(V1, hum);
    Blynk.virtualWrite(V3, mq135_calibrated);
    Blynk.virtualWrite(V4, mq2_calibrated);
    gpsServer.handleClient();
    delay(1000);
}

void connectWifi(){
    if(WiFiConnectMode == 0){
        // Operate the ESP12E in wifi station mode for smart config
        WiFi.mode(WIFI_STA);

        // Begin the smart configuration to get the Access Point credentials
        WiFi.beginSmartConfig();
        Serial.println("------------------------------------------------");
        Serial.print("Waiting for SmartConfig ");
        while (!WiFi.smartConfigDone()) {
            delay(250);
            Serial.print(".");
        }
        Serial.println();
        Serial.println("SmartConfig done.");

        // Print the AP credentials to the serial monitor
        ssid = WiFi.SSID();
        password = WiFi.psk();
        Serial.println("------------------------------------------------");
        Serial.print("Acesspoint SSID : ");
        Serial.println(ssid);
        Serial.print("Acesspoint password : ");
        Serial.println(password);
        Serial.println("------------------------------------------------");

        // Connect the ESP12E to the AP
        Serial.print("Connecting to Access Point ");
        while (WiFi.status() != WL_CONNECTED) {
            delay(100);
            Serial.print(".");
        }
        Serial.println();
        Serial.println("Connected.");
        Serial.println("------------------------------------------------");
        Serial.print("IP Address: ");
        Serial.println(WiFi.localIP());
        Serial.println("------------------------------------------------");
    }
    else{
        String ssid = "AndroidAP"; //Access point ssid
        String password = "87654321"; //Access point password
        WiFi.begin(ssid,password);
        Serial.println("------------------------------------------------");
        Serial.print("Connecting to Access Point ");
        while (WiFi.status() != WL_CONNECTED) {
            delay(500);
            Serial.print(".");
        }
        Serial.println("");
        Serial.print("Connected to ");
        Serial.println(ssid);
        Serial.print("IP address: ");
        Serial.println(WiFi.localIP());
        Serial.println("------------------------------------------------");
    }
}

void updateLatLon(){
    if (gps.encode(gpsSerial.read()))
    {  
      sat = gps.satellites.value();
      latitude = gps.location.lat();
      longitude = gps.location.lng();
      dtostrf(latitude,9,7,lati);
      dtostrf(longitude,9,7,longi);
      Serial.print("SATS: ");
      Serial.println(sat);
      Serial.print("LAT: ");
      Serial.println(latitude,6);
      Serial.print("LONG: ");
      Serial.println(longitude,6);
      Serial.print("ALT: ");
      Serial.println(gps.altitude.meters());
      Serial.print("SPEED: ");
      Serial.println(gps.speed.mps());
 
      Serial.print("Date: ");
      date = String(gps.date.day())+"/"+gps.date.month()+"/"+gps.date.year();
      Serial.println(date);
 
      Serial.print("Hour: ");
      Serial.print(gps.time.hour()); Serial.print(":");
      Serial.print(gps.time.minute()); Serial.print(":");
      Serial.println(gps.time.second());
      Serial.println("---------------------------");
      delay(100);
    }
}

void pip(){
    int fenceSize = sizeof(fences[fence - 1])/sizeof(fences[fence - 1][0]);
    double vectors[fenceSize][2];
    for(int i = 0; i < fenceSize; i++){
        vectors[i][0] = fences[fence - 1][i][0] - latitude;
        vectors[i][1] = fences[fence - 1][i][1] - longitude;
    }
    double angle = 0;
    double num, den;
    for(int i = 0; i < fenceSize; i++){
        num = (vectors[i%fenceSize][0])*(vectors[(i+1)%fenceSize][0])+ (vectors[i%fenceSize][1])*(vectors[(i+1)%fenceSize][1]);
        den = (sqrt(pow(vectors[i%fenceSize][0],2) + pow(vectors[i%fenceSize][1],2)))*(sqrt(pow(vectors[(i+1)%fenceSize][0],2) + pow(vectors[(i+1)%fenceSize][1],2)));
        angle = angle + (180*acos(num/den)/M_PI);
    }
    dtostrf(angle,9,7,cumulativeAngle);
    if(angle > 355 && angle < 365)
        targetStatus = 1;
    else
        targetStatus = 0;
}

void handleRoot(){
    String s = webpage;
    gpsServer.send(200, "text/html", s);
}

void fenceSelect(){
    fence = gpsServer.arg("fenceValue").toInt();
    gpsServer.send(200, "text/plane", String(fence));
}

void gps_data(){
    String payload = String(sat) + "#" + date + "#" + lati + "#" + longi;
    if(targetStatus == 0){
        payload = payload + "#outside";
      Blynk.logEvent("miner_in_unsafe_location", "Miner is outside the safe area. Check Location");
    }
    else
        payload = payload + "#inside";
    payload = payload + "#" + cumulativeAngle;
    if(deviceStatus == 0)
        payload = payload + "#offline";
    else
        payload = payload + "#online";
    gpsServer.send(200, "text/plane", payload);
}