
#include <SoftwareSerial.h>
#include "DHT.h"

SoftwareSerial LoRaUART(10, 11); // to TX, to RX
#define DHTPIN 2     // what pin we're connected to
#define DHTTYPE DHT11   // DHT 11
DHT dht(DHTPIN, DHTTYPE);
String sensorData;
const int DTX_LED_PIN = 13;
boolean ledState = false;

void setup() {
  Serial.begin(9600);

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
  dht.begin();
  pinMode(DTX_LED_PIN, OUTPUT);
}

void loop() {
  ledState = !ledState;
  digitalWrite(DTX_LED_PIN, ledState);
  delay(10 * 1000);
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  sensorData = String(h, 2) + "/" + String(t, 2);
  //Serial.println("value:"+sensorData);
  LoRaUART.println("AT+DTX=11,\"" + sensorData + "\"");
}
