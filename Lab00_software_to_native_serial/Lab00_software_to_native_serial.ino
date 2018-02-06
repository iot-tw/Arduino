/*
  Software serial multple serial test
 通透範例程式，讓LORA 模組通過軟體序列埠能直接轉到arduion 硬體的序列埠
 Receives from the hardware serial, sends to software serial.
 Receives from software serial, sends to hardware serial.

 The circuit:
 * RX is digital pin 10 (connect to TX of other device)
 * TX is digital pin 11 (connect to RX of other device)

 */
// LoRaSerial Port Arduino Mega 2560 //Tx2,Rx2
HardwareSerial& mySerial = Serial2;
//#include <SoftwareSerial.h>
//SoftwareSerial mySerial(10, 11); // RX, TX

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ;
  }
  Serial.println("Hardware Serial Ready");

  // set the data rate for the SoftwareSerial port
  mySerial.begin(9600);
  mySerial.println("AT+SGMD?");
}

void loop() { // run over and over
  if (mySerial.available()) {
    Serial.write(mySerial.read());
  }
  if (Serial.available()) {
    mySerial.write(Serial.read());
  }
}
