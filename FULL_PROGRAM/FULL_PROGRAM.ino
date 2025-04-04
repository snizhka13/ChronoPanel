#include "setting.h"
#include "logic.h"

void setup() {
  Serial.begin(115200);
  mySerial.begin(9600, SERIAL_8N1, PIN_MP3_RX,  PIN_MP3_TX);
  player.begin(mySerial);
  player.volume(30);
  dhtSensor.setup(DHT_PIN, DHTesp::DHT11);
  matrix.begin();
  matrix.setIntensity(5);
  matrix.displayClear();
  WiFiConnection();
}

void loop() {
  checkPlayer();   
  processQueue();  
  Alarm();
  DHT();
  MQ7();
  Breaks(); 
  Messages();
  Time();
}