#include <SoftwareSerial.h>

SoftwareSerial LoRaUART(10, 11); // to TX, to RX

unsigned long c = 1;
void setup() {
  Serial.begin(9600);
  Serial.println("Steup, Serial Ready");
  LoRaUART.begin(9600);
}

void dtx() {
  char buf[33];
  memset(buf, 0x0, 33);
  sprintf(buf, "at+dtx=8,%08d", c++);
  LoRaUART.println(buf);
  c = c%10000;
}
void loop() {
  delay(60*1000); // 60 sec
  dtx();
}
