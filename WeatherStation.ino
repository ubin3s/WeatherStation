#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>
#include <PMserial.h>
#include "DHT.h"
#include "ThingSpeak.h"

#define relay D7 // for shutdown power Aircard 
#define RXS D2
#define TXS D3
#define DHTPIN D1
#define DHT2PIN D6
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);
DHT dht2(DHT2PIN, DHTTYPE);

const char* ssid = ""; // your SSID network
const char* password = ""; // your password SSID
WiFiClient client;
unsigned long ChannelNumber = ; // thingSpeak channel number 
const char* WriteAPIKey = ""; // thingSpeak WriteAPIkey

SoftwareSerial inputSerial(RXS, TXS); // RX | TX
SerialPM pms(PMSx003, Serial);  // PMSx003, UART

int wind_dir[10] = {0, 45, 90, 135, 180, 225, 270, 315};
int wind_direction_degree = 0;
float temp;
float hum;
float temp_indoor;
float wind_speed_avg_1_minute = 0;
float wind_speed_max_5_minute = 0;
float rainfall_1_hour = 0;
float rainfall_24_hour = 0;
String inputString = "";

void setup() {
  pinMode(RXS, INPUT);
  pinMode(TXS, OUTPUT);
  pinMode(relay, OUTPUT);
  digitalWrite(relay, LOW);
  Serial.begin(9600);
  inputSerial.begin(9600);
  dht.begin();
  dht2.begin();
  pms.init();
    ThingSpeak.begin(client);
    Serial.println("Starting to Connecting");
    WiFi.begin(ssid, password);
    while(WiFi.status() != WL_CONNECTED){
      delay(1000);
      Serial.print(".");
    }
    Serial.println("WIFI Connected");
    Serial.print("IP Address:");
    Serial.println(WiFi.localIP());   
}

void loop() {
  input_read();
  SendData();
  delay(7000); // wait for ESP32CAM send picture to firebase 
  digitalWrite(relay, HIGH); 
  ESP.deepSleep(900e6); // delay 15 minute.
}

void input_read() {
  
  Serial.println("-------DHT--------");
   hum = dht.readHumidity();
   temp = dht.readTemperature();
   temp_indoor = dht2.readTemperature();
  if (isnan(temp) || isnan(hum) || isnan(temp_indoor)){
    Serial.println(F("Failed to read DHT sensor"));
    return;
  }
  Serial.print(F("Temperature: "));
  Serial.print(temp);
  Serial.println(" C");
  Serial.print(F("Humidity: "));
  Serial.print(hum);
  Serial.println(" %");
  Serial.print(F("Temperature Indoor: "));
  Serial.print(temp_indoor);
  Serial.println(" C");
 
  while (inputSerial.available()) {
    char inChar = (char)inputSerial.read();
    if (inChar == '\n') {
      decode_string(inputString);
      Serial.println("-----Wind & Rain-----");
      Serial.println("Wind direction:" + String(wind_direction_degree));
      Serial.println("Wind speed avg (1minute):" + String(wind_speed_avg_1_minute, 1) + "km/h");
      Serial.println("Wind speed max (5minute):" + String(wind_speed_max_5_minute, 1) + "km/h");
      Serial.println("Rain Fall (One Hour):" + String(rainfall_1_hour, 1) + "mm");
      Serial.println("Rain Fall (24 Hour):" + String(rainfall_24_hour, 1) + "mm");
      inputString = "";
    }
    else {
      inputString += inChar;
    }
  }
  Serial.println("----Read PMS----");
   pms.read();
   Serial.print(F("PM 2.5: "));Serial.print(pms.pm25);Serial.println(F(" ug/m3"));
}

void SendData(){
   ThingSpeak.setField(1, temp); // for outside temperature  
   ThingSpeak.setField(2, hum);
   ThingSpeak.setField(3, pms.pm25);
   ThingSpeak.setField(4, rainfall_1_hour);
   ThingSpeak.setField(5, wind_speed_avg_1_minute);
   ThingSpeak.setField(6, rainfall_24_hour);
   ThingSpeak.setField(7, wind_direction_degree);
   ThingSpeak.setField(8, temp_indoor); // Temperature inside Weather station.
   int x = ThingSpeak.writeFields(ChannelNumber, WriteAPIKey);
   if(x == 200){
    Serial.println("Channel update Successful.");
   }else{
    Serial.println("Problem updating. HTTP error code" + String(x));
   }
}

void decode_string(String code) {
  wind_direction_degree = code.substring(1, 4).toInt();
  wind_speed_avg_1_minute = code.substring(5, 8).toInt();
  wind_speed_avg_1_minute *= 1.609344;
  wind_speed_max_5_minute = code.substring(9, 12).toInt();
  wind_speed_max_5_minute *= 1.609344;
  rainfall_1_hour = code.substring(17, 20).toInt();
  rainfall_1_hour *= 0.254;
  rainfall_24_hour = code.substring(21, 24).toInt();
  rainfall_24_hour *= 0.254;
}
