/*
  This is a simple example show the Heltec.LoRa recived data in OLED.

  The onboard OLED display is SSD1306 driver and I2C interface. In order to make the
  OLED correctly operation, you should output a high-low-high(1-0-1) signal by soft-
  ware to OLED's reset pin, the low-level signal at least 5ms.

  OLED pins to ESP32 GPIOs via this connecthin:
  OLED_SDA -- GPIO4
  OLED_SCL -- GPIO15
  OLED_RST -- GPIO16
  
  by Aaron.Lee from HelTec AutoMation, ChengDu, China
  成都惠利特自动化科技有限公司
  www.heltec.cn
  
  This project is also available on GitHub:
  https://github.com/Heltec-Aaron-Lee/WiFi_Kit_series
*/
#include "heltec.h" 
#include "images.h"

#define BAND    868E6  //you can set band here directly,e.g. 868E6,915E6
String rssi = "RSSI --";
String packSize = "--";
String packet ;
int pkt_limit = 100;
int pkt = 0;
int rssi_sum = 0;
float avg_rssi = 0;
float reference_rssi = 0;
//float gamma = 2;
float est_distance = 0;
//float expo = 0;

void LoRaData(){
  Heltec.display->clear();
  Heltec.display->setTextAlignment(TEXT_ALIGN_LEFT);
  Heltec.display->setFont(ArialMT_Plain_10);
  Heltec.display->drawString(0 , 12 , "Received "+ packSize + " bytes");
  Heltec.display->drawStringMaxWidth(0 , 23 , 128, packet);
  Heltec.display->drawString(0, 0, rssi);  
  if (est_distance==0){
    Heltec.display->drawString(0, 33, "Calibrating Reference RSSI");
  }
  else{
    Heltec.display->drawString(0, 33, "Est distance: "+ String(est_distance) + " m");
  }
  Heltec.display->display();
}


void distance(){
  float gamma = 2;
  float expo = (reference_rssi - avg_rssi)/(10.0 * gamma);
  est_distance = 2 * pow(10, expo);
  Serial.println("Estimated distance is: "+ String(est_distance));
}

void onReceive(int packetSize) {
  packet ="";
  packSize = String(packetSize,DEC);
  for (int i = 0; i < packetSize; i++) { packet += (char) LoRa.read(); }
  rssi = "RSSI " + String(LoRa.packetRssi(), DEC) ;
  Serial.println("RSSI of pkt: " + String(packet) + " is: " + String(LoRa.packetRssi()));
  //Serial.println("Radio RSSI: " + String(LoRa.rssi()));
  if (pkt<pkt_limit){
    rssi_sum += LoRa.packetRssi();
    pkt += 1;
  }
  if (pkt==pkt_limit){
    if (pkt_limit == 100){
      // Got calibration value
      reference_rssi = rssi_sum/pkt_limit;
      Serial.println("Calibration RSSI: " + String(reference_rssi));
    }
    else{
      // Got last AVG value
      avg_rssi = rssi_sum/pkt_limit;
      Serial.println("AVG RSSI of last: " + String(pkt) + " packets is: " + String(avg_rssi));
      // Call distance estimation
      distance();
    }
    pkt = 0;
    rssi_sum = 0;
    pkt_limit = 10;
  }
  LoRaData();
}

void setup() { 
   //WIFI Kit series V1 not support Vext control
  //LoRa.setSpreadingFactor(7); 
  //LoRa.setSyncWord(0x34); 
  Heltec.begin(true /*DisplayEnable Enable*/, true /*Heltec.Heltec.Heltec.LoRa Disable*/, true /*Serial Enable*/, true /*PABOOST Enable*/, BAND /*long BAND*/);
 
  Heltec.display->init();
  Heltec.display->flipScreenVertically();  
  Heltec.display->setFont(ArialMT_Plain_10);

  delay(500);
  Heltec.display->clear();
  
  Heltec.display->drawString(0, 0, "Heltec.LoRa Initial success!");
  Heltec.display->drawString(0, 10, "Wait for incoming data...");
  Heltec.display->display();
  delay(500);
  //LoRa.onReceive(cbk);
  //LoRa.setSignalBandwidth(250);
  LoRa.onReceive(onReceive);
  //LoRa.setGain(1);
  LoRa.setSpreadingFactor(7);
  LoRa.receive();
  delay(2500);
}

void loop() {
  Serial.println("Running");
  delay(5000);
}
