#include <SPI.h>
#include <MFRC522.h>
#include <stdio.h>
#include <DS1302.h>
#include <Wire.h>
#include <SoftwareSerial.h>
// DS1302 clock 模組 Arduino PIN 定義
const int kCePin   = 4;  //RST
const int kIoPin   = 5;  //DAT
const int kSclkPin = 6;  //CLK

DS1302 rtc(kCePin, kIoPin, kSclkPin);
// RFID Reader RFID-RC522模組 PIN 定義
#define RST_PIN         9          // Configurable, see typical pin layout above
#define SS_PIN          10         // Configurable, see typical pin layout above
//下面三個port 不能改     UNO        MEGA      NanoV3      Micro          Micro Pro
//SPI MOSI    MOSI         11         51        D11        ICSP-4           16
//SPI MISO    MISO         12         50        D12        ICSP-1           14
//SPI SCK     SCK          13         52        D13        ICSP-3           15
MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance
//蜂鳴器 PIN 定義
int Buzzerpin = 2;
//lora module PIN define
SoftwareSerial LoRaUART(7,8); // to TX, to RX



String dayAsString(const Time::Day day) {
  switch (day) {
    case Time::kSunday: return "Sunday";
    case Time::kMonday: return "Monday";
    case Time::kTuesday: return "Tuesday";
    case Time::kWednesday: return "Wednesday";
    case Time::kThursday: return "Thursday";
    case Time::kFriday: return "Friday";
    case Time::kSaturday: return "Saturday";
  }
  return "(unknown day)";
}
String printTime() {
  // Get the current time and date from the chip.
  Time t = rtc.time();

  // Name the day of the week.
  const String day = dayAsString(t.day);

  // Format the time and date and insert into the temporary buffer.
  char buf[50];
  char lora_time[11];
  snprintf(buf, sizeof(buf), "%s %04d-%02d-%02d %02d:%02d:%02d",
           day.c_str(),
           t.yr, t.mon, t.date,
           t.hr, t.min, t.sec);
  snprintf(lora_time, sizeof(lora_time), "%02d%02d%02d%02d%02d",
           t.mon, t.date,
           t.hr, t.min, t.sec);
  // Print the formatted string to serial so we can see the time.
  Serial.println(buf);
  return lora_time;
}
void setup() {
  Serial.begin(9600);   // Initialize serial communications with the PC
  while (!Serial);    // Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4)
  LoRaUART.begin(9600);
  LoRaUART.println("AT+SGMD?");
  //  while (!LoRaUART.available()) {
  //  Serial.println("wating for LoRaUART ready.....");
  //}
  Serial.println("LoRa Serial Ready");
  
  SPI.begin();      // Init SPI bus
  mfrc522.PCD_Init();   // Init MFRC522
  mfrc522.PCD_DumpVersionToSerial();  // Show details of PCD - MFRC522 Card Reader details
  Serial.println(F("Scan PICC to see UID, SAK, type, and data blocks..."));
  pinMode(Buzzerpin, OUTPUT);     
  digitalWrite(Buzzerpin,LOW);     //蜂鳴器初始為不鳴叫  
    
}

void loop() {
    // 確認是否有新卡片
    if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
      byte *id = mfrc522.uid.uidByte;   // 取得卡片的UID
      byte idSize = mfrc522.uid.size;   // 取得UID的長度
 
      Serial.print("PICC type: ");      // 顯示卡片類型
      // 根據卡片回應的SAK值（mfrc522.uid.sak）判斷卡片類型
      MFRC522::PICC_Type piccType = mfrc522.PICC_GetType(mfrc522.uid.sak);
      Serial.println(mfrc522.PICC_GetTypeName(piccType));
 
      Serial.print("UID Size: ");       // 顯示卡片的UID長度值
      Serial.println(idSize);
      char tmp;
      for (byte i = 0; i < idSize; i++) {  // 逐一顯示UID碼
        Serial.print("id[");
        Serial.print(i);
        Serial.print("]: ");
        Serial.println(id[i], HEX);       // 以16進位顯示UID值
        //sprintf(tmp, "0x%.2X",id[i]);
        //Serial.println(tmp);
      }
      String hexString0 = String(id[0],HEX);
      String hexString1 = String(id[1],HEX);
      String hexString2 = String(id[2],HEX);
      String hexString3 = String(id[3],HEX);
      if (String(id[0],HEX).length()==1){
        hexString0 = "0" + String(id[0],HEX);
        }
      if (String(id[1],HEX).length()==1){
        hexString1 = "0" + String(id[1],HEX);
        }
      if (String(id[2],HEX).length()==1){
        hexString2 = "0" + String(id[2],HEX);
        }
      if (String(id[3],HEX).length()==1){
        hexString3 = "0" + String(id[3],HEX);
        }
      String lora_TagID = hexString0 + hexString1 + hexString2 + hexString3;
      String lora_time = printTime();
      Serial.println(lora_TagID+lora_time);
      //Serial.println((lora_TagID+lora_time).length());
      LoRaUART.println("AT+DTX=18,"+lora_TagID+lora_time);
      Serial.println();
      //Serial.println(lora_time);
      digitalWrite(Buzzerpin,HIGH); //啟動蜂鳴器
      delay(30);
      mfrc522.PICC_HaltA();  // 讓卡片進入停止模式
      digitalWrite(Buzzerpin,LOW);   ////關閉蜂鳴器
      
    }  
}
