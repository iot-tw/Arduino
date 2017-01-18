#include <SoftwareSerial.h>
#define SERIAL_BAUDRATE 9600
#define CLK_PIN 2 // 定義連接腳位
#define DT_PIN 3
#define SW_PIN 4
SoftwareSerial LoRaUART(10, 11); // to TX, to RX
#define interruptA 0 // UNO腳位2是interrupt 0，其他板子請見官方網頁

volatile long SensorValue = 0;
unsigned long t = 0;

void setup() {
  Serial.begin(SERIAL_BAUDRATE);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  Serial.println("Serial Ready");
  LoRaUART.begin(SERIAL_BAUDRATE);
  LoRaUART.println("AT");

  while (!LoRaUART.available()) {
    ; // wait for software serial port ready.
  }
  Serial.println("LoRa Serial Ready");
  // 當狀態下降時，代表旋轉編碼器被轉動了
  attachInterrupt(interruptA, rotaryEncoderChanged, FALLING);
  pinMode(CLK_PIN, INPUT_PULLUP); // 輸入模式並啟用內建上拉電阻
  pinMode(DT_PIN, INPUT_PULLUP); 
  pinMode(SW_PIN, INPUT_PULLUP); 
}
void loop() {
  if(digitalRead(SW_PIN) == LOW){ // 按下開關，歸零
     SensorValue = 0;  
     Serial.println("count reset to 0");
     delay(300);
  }
}
void rotaryEncoderChanged(){ // when CLK_PIN is FALLING
  unsigned long temp = millis();
  if(temp - t < 200) // 去彈跳
    return;
  t = temp;
  
  // DT_PIN的狀態代表正轉或逆轉
  SensorValue += digitalRead(DT_PIN) == HIGH ? 1 : -1;
  String SensorData;
  SensorData = String(SensorValue);
  Serial.println(SensorData);
  LoRaUART.println("AT+DTX=" + String(SensorData.length()) + ",\"" + SensorData + "\"");
}
