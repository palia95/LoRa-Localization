//Libraries for LoRa
#include <SPI.h>
#include <LoRa.h>
#include <WiFi.h>

#define SSD1306_ADDRESS 0x3C

//define the pins used by the LoRa transceiver module
#define SCK 5
#define MISO 19
#define MOSI 27
#define SS 18
#define RST 23
#define DIO0 26
#define I2C_SDA 21
#define I2C_SCL 22

//433E6 for Asia
//866E6 for Europe
//915E6 for North America
#define BAND 868E6
#define BW 250E3
#define CH_SCAN 100 //ms

String packSize = "--";
String packet = "";
float rssi = 0.0;
float snr = 0.0;
unsigned int pkt_limit = 200;
unsigned int pkt = 0;
float rssi_sum = 0.0;
float reference_rssi = 0.0;
float avg_rssi = 0.0;
int calib_distance = 1;
float est_distance = 0.0;
//float expo = 0;

//Start WiFi AP
void startWiFi(){
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
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
  //LoRa.onReceive(onReceive);
  LoRa.setGain(1);
  LoRa.receive();
}

void distance(){
  float gamma = 2.2;
  float expo = (reference_rssi - avg_rssi)/(10.0 * gamma);
  est_distance = calib_distance * pow(10, expo);
  Serial.println("Estimated distance is: "+ String(est_distance));
}

void Receive(int packetSize) {
  packet ="";
  packSize = String(packetSize,DEC);
  rssi = LoRa.packetRssi();
  snr = LoRa.packetSnr();
  for (int i = 0; i < packetSize; i++) { packet += (char) LoRa.read(); }
  //write to the serial logger LORA:ID:RSSI:SNR
  Serial.println("LORA:"+String(packet)+":"+String(rssi)+":"+String(snr));
  //Serial.println("SNR of pkt is: " + String(snr));
  //Serial.println("Freq Error of pkt: " + String(packet) + " is: " + String(LoRa.packetFrequencyError()));
  
//  if (pkt<pkt_limit){
//    rssi_sum += rssi;
//    pkt += 1;
//  }
//  if (pkt==pkt_limit){
//    if (pkt_limit == 200){
//      // Got calibration value
//      reference_rssi = rssi_sum/pkt_limit;
//      Serial.println("Calibration RSSI: " + String(reference_rssi));
//    }
//    else{
//      // Got last AVG value
//      avg_rssi = rssi_sum/pkt_limit;
//      Serial.println("AVG RSSI of last: " + String(pkt) + " packets is: " + String(avg_rssi));
//      // Call distance estimation
//      distance();
//    }
//    //Serial.println("Updating variable");
//    pkt = 0;
//    rssi_sum = 0.0;
//    pkt_limit = 100;
//  }
  //screen_print();
}

void setup() { 
  //initialize Serial Monitor
  Serial.begin(115200);
  
  // Display
  //screen_setup();
  startLoRA();
  startWiFi();
  delay(500);
  WiFi.scanNetworks(true, false, CH_SCAN);
}

void printScanResult(int networksFound)
{
  //Serial.printf("%d network(s) found\n", networksFound);
  for (int i = 0; i < networksFound; i++)
  {
    //Serial.printf("%d: %s, Ch:%d (%ddBm) \n", i + 1, WiFi.SSID(i).c_str(), WiFi.channel(i), WiFi.RSSI(i));
    //Serial.printf("%d: %s, (%ddBm) \n", i + 1, WiFi.SSID(i).c_str(), WiFi.RSSI(i));
    //write to the serial logger WIFI:SSID:RSSI:CH
    Serial.println("WIFI:"+String(WiFi.SSID(i))+":"+String(WiFi.RSSI(i))+":"+String(WiFi.channel(i)));
    
  }
  // Delete the scan result to free memory for code below.
  WiFi.scanDelete();
  WiFi.scanNetworks(true, false, CH_SCAN);
}

void loop() {
  int packetSize = LoRa.parsePacket();
  if (packetSize) { 
    Receive(packetSize); 
  }
  int networksFound = WiFi.scanComplete();
  if (networksFound > 0){
    printScanResult(networksFound);   
  }
  //delay(500);
}
