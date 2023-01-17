volatile unsigned long startCountVar = 0;
volatile unsigned long stopCountVar = 0;
unsigned long delay_offset = 136;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
}

void startCount(){
  //Serial.println("Clock count start");
  startCountVar = ESP.getCycleCount();
}

void stopCount(){
  //Serial.println("Clock count stop");
  stopCountVar = ESP.getCycleCount();
  //ESP.restart();
}

void loop() {
  // put your main code here, to run repeatedly:
  startCountVar = ESP.getCycleCount();
  delayMicroseconds(1); // one microsecond to mesaure software overlay
  unsigned int count = ESP.getCycleCount() - startCountVar - delay_offset;
  Serial.println(count);
  delay(1000);

}
