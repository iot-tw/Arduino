/*
利用Max485 IC 轉換UART 為RS485，接上一個DC電錶，提供電壓與溫度
使用Serial3 來控制 電錶 Tx3 RX3
使用PIN D3 來控制MAX485 IC
使用Serial2 來發送LoRa信號
*/

#include <ModbusMaster.h>

/*!
  We're using a MAX485-compatible RS485 Transceiver.
  Rx/Tx is hooked up to the hardware serial port at 'Serial3'.
  The Data Enable and Receiver Enable pins are hooked up as follows:
*/

//in my case,i let DE & RE jump together to Pin 3
#define MAX485_DE      3
#define MAX485_RE_NEG  3

HardwareSerial& LoRaUART = Serial2;
// instantiate ModbusMaster object
ModbusMaster node;

void preTransmission()
{
  digitalWrite(MAX485_RE_NEG, 1);
  digitalWrite(MAX485_DE, 1);
}

void postTransmission()
{
  digitalWrite(MAX485_RE_NEG, 0);
  digitalWrite(MAX485_DE, 0);
}

void setup()
{
  Serial.begin(9600);
  while (!Serial) {
    ;
  }
  Serial.println("Hardware Serial Ready");
  // Modbus communication runs at 9600 baud
  Serial3.begin(9600);
  
  while (!Serial3) {    ;  }
  Serial.println("RS485 Module Serial Ready");
  
  LoRaUART.begin(9600);
  while (!LoRaUART) {    ;  }
  Serial.println("LoRa Module Serial Ready");  

  LoRaUART.println("at+sgmd?");
  while (!LoRaUART.available()) {;}
  String myString = LoRaUART.readString();
  myString.replace("\n","");
  Serial.println("sgmd[" + myString + "]");

  LoRaUART.println("at+dttx");
  while (!LoRaUART.available()) {;} 
  myString = LoRaUART.readString();
  myString.replace("\n","");
  Serial.println("dttx[" + myString +"]");
  
  pinMode(MAX485_RE_NEG, OUTPUT);
  pinMode(MAX485_DE, OUTPUT);
  // Init in receive mode
  digitalWrite(MAX485_RE_NEG, 0);
  digitalWrite(MAX485_DE, 0);

  // Modbus slave ID 1 
  node.begin(1, Serial3);

  
  // Callbacks allow us to configure the RS485 transceiver correctly
  node.preTransmission(preTransmission);
  node.postTransmission(postTransmission);
}

bool state = true;

void loop()
{
  uint8_t result;
  uint16_t data[6];
  
  

  // Read 4 registers starting at 0x0000)
  result = node.readHoldingRegisters(0x0000, 4);
  if (result == node.ku8MBSuccess)
  {
    Serial.println("SlaveValue:"); 
    Serial.println("TEST");
    int Voltage = (node.getResponseBuffer(0x00));
    String LoRa_Voltage = String(Voltage, HEX);
    Serial.println("Voltage/100:" + String(Voltage));
    Serial.println("Voltage HEX:" + LoRa_Voltage);

    int Ampere = node.getResponseBuffer(0x01);
    String LoRa_Ampere = String(Ampere, HEX);
    Serial.println("Ampere/100 :" + String(Ampere));
    Serial.println("Ampere Hex:" + LoRa_Ampere);

    int Celsius = node.getResponseBuffer(0x02);
    String LoRa_Celsius = String(Celsius, HEX);
    Serial.println("Celsius:" + String(Celsius));
    Serial.println("Celsius HEX:" + LoRa_Celsius);
    
    //Serial.println(node.getResponseBuffer(0x00));
    //Serial.println(node.getResponseBuffer(0x01));
    //Serial.println(node.getResponseBuffer(0x02));
    //Serial.println(node.getResponseBuffer(0x03));
    LoRaUART.println("at+dtx=6,0" +LoRa_Voltage+LoRa_Celsius);
    Serial.println("Debug AT: at+dtx=6,0"+LoRa_Voltage+LoRa_Celsius);
  
  //LoRaUART.println("at+dttx");
  while (!LoRaUART.available()) {;} 
  String myString = LoRaUART.readString();
  myString.replace("\n","");
  Serial.println("dtx[" + myString +"]");
     
  }

  delay(5000);
}


