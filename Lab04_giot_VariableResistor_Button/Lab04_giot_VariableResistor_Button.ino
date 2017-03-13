//讀取A0 電壓，讀取 Pin 4 High,beep PIN 9，/Low ,LoRa 發送出去
#include <SoftwareSerial.h>

SoftwareSerial LoRaUART(10, 11); // to TX, to RX
const int variable = A0;
const int beep = 9;
const int button = 4;
int btVoltage = 0;
void setup() {
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(button, INPUT);
  pinMode(beep, OUTPUT);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
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

  btVoltage = digitalRead(button);

  if (btVoltage == HIGH){
    digitalWrite(LED_BUILTIN, HIGH);
    tone(beep, 262, 200);
    Serial.println("Push down");
    Serial.println("AT+DTX=" + String(sensorData.length()+1) + ",\"" + "B" + sensorData + "\"");
    LoRaUART.println("AT+DTX=" + String(sensorData.length()+1) + ",\"" + "B" + sensorData + "\"");
  }
  else{
    digitalWrite(LED_BUILTIN, LOW);
    digitalWrite(9, LOW);
    Serial.println("Button up");
    tone(beep, sensorValue, 200);
    Serial.println("AT+DTX=" + String(sensorData.length()+1) + ",\"" + "R" + sensorData + "\"");
    LoRaUART.println("AT+DTX=" + String(sensorData.length()+1) + ",\"" + "R" + sensorData + "\"");
  }
  delay(5*1000);
}
