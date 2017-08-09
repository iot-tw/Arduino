// include the library code:
// 給 LCM1602_IIC_V1 + 16x2 LCD 用的程式庫
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);
#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

// LoRaSerial Port Arduino Mega 2560 //Tx1,Rx1
HardwareSerial& LoRaUART = Serial1;
// LoRaSerial Port Arduino UNO,這樣接SoftwareSerial Rx 會有亂碼問題，導致Downlink 接收錯誤。
//#include <SoftwareSerial.h>
//SoftwareSerial LoRaUART(10, 11); // to Tx, To Rx
// 真要接，得把用在Debug 的Serial Monitor 的Serial Tx0 Rx0 拿來用，然後關掉debug messages,
// Sketch 代碼時 得把LoRa 接UNO 上的 Rx D0 Pin 拿下，不然upload 會失敗。
//HardwareSerial& LoRaUART = Serial;

void setup()
{
  String myString;
  // set up the LCD's number of columns and rows:
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3D (for the 128x64)
  display.clearDisplay();   // clears the screen and buffer
  // Print a message to the LCD.
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println("LoRa Class C DownLink Example");
  display.display();
  delay(2000);
  
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  Serial.println("Serial Ready");
  LoRaUART.begin(9600);
     while (!LoRaUART) {
       ; // wait for serial1 port to connect.
     }
  LoRaUART.println("at+echo=1");
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
    if (Len >= 10){
      RSSI = myString.substring(26,30);
    }
    else {
      RSSI = myString.substring(25,29);
    }
    SNR = myString.substring(33,36);
    Serial.println("Len="+String(Len)+" RSSI="+RSSI+" SNR="+SNR);
    LoRaUART.println("at+DRX?");
    while (!LoRaUART.available()) {;}
    DownLink = LoRaUART.readString();
    DownLink.trim();
    Serial.println("DRX["+DownLink+"]");
    Length = DownLink.indexOf(",");
    PayLoad = DownLink.substring(Length+1,Length+Len*2+1);
    Serial.println("PayLoad="+PayLoad);
  display.clearDisplay();   // Clear the buffer.
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println("Len:"+String(Len)+" RSSI:"+ RSSI+" SNR:"+SNR);
  //display.println("RSSI:"+ RSSI);
  display.setTextSize(2);
  display.println(PayLoad);
  display.display();
  delay(2000);
  }
}
