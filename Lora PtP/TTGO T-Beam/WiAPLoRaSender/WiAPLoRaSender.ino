/*********
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp32-lora-sensor-web-server/
  
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.
  
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*********/

//Libraries for LoRa
#include <SPI.h>
#include <LoRa.h>
#include <WiFi.h>
#include <WiFiAP.h>

// Set these to your desired credentials.
const char *ssid = "AP-18";
const char *password = "Hermes2022";
int channel = 12;

//define the pins used by the LoRa transceiver module
#define SCK 5
#define MISO 19
#define MOSI 27
#define SS 18
#define RST 23
#define DIO0 26

//433E6 for Asia
//866E6 for Europe
//915E6 for North America
#define BAND 868E6
#define BW 250E3

//packet counter
int readingID = 0;
int Node_ID = 18;
int counter = 0;
String LoRaMessage = "";

//Start WiFi AP
void startWiFi(){
  // You can remove the password parameter if you want the AP to be open.
  WiFi.softAP(ssid, password, channel);
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
}

//Initialize LoRa module
void startLoRA(){
  //SPI LoRa pins
  SPI.begin(SCK, MISO, MOSI, SS);
  //setup LoRa transceiver module
  LoRa.setPins(SS, RST, DIO0);

  if (!LoRa.begin(BAND)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  LoRa.setSpreadingFactor(7);
  LoRa.setSyncWord(0x44);
  LoRa.setTxPower(20);
  LoRa.setSignalBandwidth(BW);
}

void sendData() {
  //Serial.println("Radio RSSI: " + String(LoRa.rssi()));
  LoRaMessage = String(Node_ID);//+"@"+String(readingID);
  //Send LoRa packet to receiver
  LoRa.beginPacket();
  LoRa.print(LoRaMessage);
  LoRa.endPacket();
  Serial.println("Sending packet: "+String(readingID));
  readingID++;
}

void setup() {
  //initialize Serial Monitor
  Serial.begin(115200);
  startLoRA();
  startWiFi();
}

void loop() {
  sendData();
  delay(100);
}
