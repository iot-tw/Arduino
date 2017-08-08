//#include <SoftwareSerial.h>
#include <Wire.h>
// include the library code:
//#include <LiquidCrystal.h>
#include <LiquidCrystal_I2C.h>
// initialize the library with the numbers of the interface pins
//LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
LiquidCrystal_I2C lcd(0x3F, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // 設定 LCD I2C 位址
// LoRaSerial Port Arduino Mega 2560 //Tx1,Rx1
HardwareSerial& LoRaUART = Serial1;
// LoRaSerial Port Arduino UNO 
//SoftwareSerial LoRaUART(10, 11); // to Tx, To Rx

void setup()
{
  String myString;
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("hello, world!");
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  Serial.println("Serial Ready");
  LoRaUART.begin(9600);
     while (!LoRaUART) {
       ; // wait for serial1 port to connect.
     }
  LoRaUART.println("at+echo=0");
     while (!LoRaUART.available()) {
      ;
     }
  if (((LoRaUART.available())) > 0)
  { //判斷有沒有訊息接收 
    myString = LoRaUART.readString();
    myString.trim();
    Serial.println("echo=0[" + myString +"]");
  }
  LoRaUART.println("at+sgmd?");
  while (!LoRaUART.available()) {;}
  myString = LoRaUART.readString();
  myString.trim(); 
  Serial.println("sgmd[" + myString + "]");
  
  LoRaUART.println("at+dttx");
  while (!LoRaUART.available()) {;} 
  myString = LoRaUART.readString();
  myString.trim(); 
  Serial.println("dttx[" + myString +"]");
  
  LoRaUART.println("at+drx?");
  while (!LoRaUART.available()) {;} 
  myString = LoRaUART.readString();
  myString.trim(); 
  Serial.println("drx[" + myString +"]");
  
  if (myString.indexOf("Radio Tx Done")) {
      Serial.println("LoRa Module Tx Done");
  }
 
} //setup()

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
  }
}
