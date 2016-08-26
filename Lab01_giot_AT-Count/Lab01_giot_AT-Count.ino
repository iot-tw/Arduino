#include <SoftwareSerial.h>

SoftwareSerial LoRaUART(10, 11); // to TX, to RX
int count = 10000;
void setup() {
  Serial.begin(9600);

  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  Serial.println("Serial Ready");

  LoRaUART.begin(9600);
  // show the module information
  LoRaUART.println("AT+SGMD?");
  // enable UART echo
  // LoRaUART.println("AT+ECHO");
  // LoRaUART.println("AT");

  while (!LoRaUART.available()) {
    ;
  }
  Serial.println("LoRa Serial Ready");
}

void loop() {

  String count_data;
  count_data = String(count);
  //LoRaUART.println("AT+DTX=6,\"abcdef\"");
  
  LoRaUART.println("AT+DTX=5,\"" + count_data + "\"" );
  Serial.println("ATDTX " + count_data );
  //LoRaUART.println("AT+DTX=16,1234567890abcdef");
  count = count + 1;
  delay(5*1000);
}
