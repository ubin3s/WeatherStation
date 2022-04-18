# Weather Station
DIY ESP8266 Micro controller weather station for Ecotourism and display on Line chatbots. It can measure Particulate matter (PM2.5), temperature, Humidity, Rainfall, Wind Speed and Wind vand.
This Project use ESP8266 DeepSleep function to minimun battery usage and wake up every 15 minute.

![ws](https://user-images.githubusercontent.com/103322325/163827383-d2ac7fb5-01fe-405f-b878-f5cdee3ba517.jpg)

## Features
- Waterproof
- Works with Line chatbots support on Smartphone and Computer.
- Can work in places there is no WiFi and Power.
- Low power: avalable without sunlight for 3-4 days. in case rain season.
- Auto sleep mode 

## Components
- ESP8266 Micro Controller 
- DHT22 Temperature and Humidity *2 (for outside and insider weather station)
- PMS5003 Particulate matter sensor 
- Relay Modules for Shutdown power
- Rain and Wind Modules 
- Network Module (Aircard or Wifi)
- 18W Solar panel 
- Solar Controller 
- 5.5 Ah battery
- Waterproof case
- USB Hub
- PCB, Pin header, Wire, Solder etc.

## library on Arduino IDE
- [DHT](https://github.com/adafruit/DHT-sensor-library)
- [PMSx00x](https://github.com/fu-hsi/PMS)
- [ThingSpeak](https://github.com/mathworks/thingspeak-arduino)
- [SoftwareSerial](https://www.arduino.cc/en/Reference/softwareSerial)

## Results
### ThingSpeak 
![result](https://user-images.githubusercontent.com/103322325/163831121-8fca0c46-e75a-4f17-8562-de75ff469f6d.png)

### Line Chatbot
![rsz_chatbots](https://user-images.githubusercontent.com/103322325/163832278-09f83c14-84a4-4775-aaaf-d5719a1f40e3.jpg)

