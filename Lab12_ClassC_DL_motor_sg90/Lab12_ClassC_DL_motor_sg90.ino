//Class C Down Link LoRaWan Command to motor Servo SG-90  
//GL6509 use Mega Serail2/Rx2
// LoRaSerial Port Arduino Mega 2560 //Tx2,Rx2
HardwareSerial& LoRaUART = Serial2;
#include <Servo.h> 
Servo myservo;
//i2c LCD use D20 D21 PIN
//include the library code:
// 給 LCM1602_IIC_V1 + 16x2 LCD 用的程式庫
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
// initialize the library with the numbers of the interface pins
LiquidCrystal_I2C lcd(0x3F, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // 設定 LCD I2C 位址 通常是 0x27 ,也有 0x3F.

void setup() 
{ 
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
    while (!Serial) {
    ;
  }
  Serial.println("Hardware Serial Ready");
    LoRaUART.begin(9600);
  while (!LoRaUART) {
    ;
  }
  Serial.println("LoRa Module Serial Ready");
  LoRaUART.begin(9600);
    while (!LoRaUART) {
      ; // wait for serial1 port to connect.
    }
  
  LoRaUART.println("atz");
  LoRaUART.println("at+echo=0");
  while (!LoRaUART.available()) {;}
  String myString = LoRaUART.readString();
  myString.replace("\n","");
  Serial.println("echo=0[" + myString +"]");
  
  myservo.attach(9, 500, 2400); // 修正脈衝寬度範圍
  Serial.println("roll 90");
  myservo.write(90); // 一開始先置中90度
  delay(3000);
  Serial.println("roll to 180");
  myservo.write(180);
  delay(3000);
} 
void loop()
{
  int Len;
  String myString;
  String DownLink;
  String PayLoad;
  String RSSI,SNR;
  int Length;
  while (!LoRaUART.available()) {;} 
  myString = LoRaUART.readString();
  if (myString.indexOf("Radio Rx Done")>=0) {
    myString.trim();
    Serial.println("Got Downlink Data["+myString+"]");
    Len = myString.substring(18,20).toInt();
    RSSI = myString.substring(25,28);
    SNR = myString.substring(33,35);
    Serial.println("Len="+String(Len)+" RSSI="+RSSI+" SNR="+SNR);
    LoRaUART.println("at+DRX?");
    while (!LoRaUART.available()) {;}
    DownLink = LoRaUART.readString();
    DownLink.trim();
    Serial.println("DRX["+DownLink+"]");
    Length = DownLink.indexOf(",");
    PayLoad = DownLink.substring(Length+1,Length+Len*2+1);
    Serial.println("PayLoad="+PayLoad);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Len:"+String(Len)+" RSSI:"+ RSSI);
    lcd.setCursor(0, 1);
    lcd.print(PayLoad);
    if (PayLoad == "0180") {
      Serial.println("roll to 0");
      myservo.write(0);
      delay(3000);
    }
    else if (PayLoad == "0000") {
      Serial.println("roll to 180");
      myservo.write(180);
      delay(3000);
    }
    }
  }
