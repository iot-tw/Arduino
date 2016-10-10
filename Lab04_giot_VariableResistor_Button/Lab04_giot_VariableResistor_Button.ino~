#include <SoftwareSerial.h>

SoftwareSerial LoRaUART(10, 11); // to TX, to RX
const int variable = A2;
const int led = 9;
const int button = 4;
int btVoltage = 0;
void setup() {
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(4, INPUT);
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
  sensorData = String(sensorValue,4);
  Serial.println(sensorData);
  
  btVoltage = digitalRead(button);
    
  if (btVoltage == HIGH){
    digitalWrite(LED_BUILTIN, HIGH);
    //Serial.println("Push down");
    Serial.println("AT+DTX=" + String(sensorData.length()+1) + ",\"" + "B" + sensorData + "\"");
    LoRaUART.println("AT+DTX=" + String(sensorData.length()+1) + ",\"" + "B" + sensorData + "\"");
  }
  else{
    digitalWrite(LED_BUILTIN, LOW);
    //Serial.println("Button up");
    Serial.println("AT+DTX=" + String(sensorData.length()+1) + ",\"" + "R" + sensorData + "\"");
    LoRaUART.println("AT+DTX=" + String(sensorData.length()+1) + ",\"" + "R" + sensorData + "\"");
  }
  delay(5*1000);
}
