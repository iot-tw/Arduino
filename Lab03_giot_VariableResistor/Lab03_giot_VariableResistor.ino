//讀取 A0 的電壓資料，利用LoRa 發送出去。
#include <SoftwareSerial.h>

SoftwareSerial LoRaUART(10, 11); // to TX, to RX
const int variable = A0;

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ; 
  }
  Serial.println("Serial Ready");

  LoRaUART.begin(9600);
  LoRaUART.println("AT");

  while (!LoRaUART.available()) {
    ;
  }
  Serial.println("LoRa Serial Ready");
}

void loop() {
  int sensorValue = analogRead(variable);
  String sensorData;
  sensorData = String(sensorValue);
  Serial.println(sensorData);
  LoRaUART.println("AT+DTX=" + String(sensorData.length()) + ",\"" + sensorData + "\"");

  delay(3*1000);
}
