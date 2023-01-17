#include <SPI.h>
#include <SD.h>

// HSPI SD pins
#define HSPI_SCLK 14
#define HSPI_MISO 4
#define HSPI_MOSI 13
#define HSPI_CS 2

//SD class
SPIClass * hspi = NULL;

// File object to represent file
File txtFile;

// file name to use for writing
const char filename[] = "LoRaRSSIdata.txt";
const int chipSelect = 4;

//init SD
void startSD(){
  hspi = new SPIClass(HSPI);
  hspi->begin(HSPI_SCLK, HSPI_MISO, HSPI_MOSI, HSPI_CS); //SCLK, MISO, MOSI, SS
  pinMode(HSPI_CS, OUTPUT); //HSPI SS

  // see if the card is present and can be initialized:
  if (!SD.begin(HSPI_CS)) {
    Serial.println("Card failed, or not present");
  }

}

void setup() {
  // put your setup code here, to run once:
  //initialize Serial Monitor
  Serial.begin(115200);
  startSD();
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("SD not found");
  delay(10000);
}
