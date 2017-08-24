/*
  Software serial multple serial test
 通透範例程式，讓LORA 模組通過軟體序列埠能直接轉到arduion 硬體的序列埠
 Receives from the hardware serial, sends to software serial.
 Receives from software serial, sends to hardware serial.

 The circuit:
 * RX is digital pin 10 (connect to TX of other device)
 * TX is digital pin 11 (connect to RX of other device)

 */
#include <SoftwareSerial.h>
#include <TinyGPS.h>
TinyGPS gps;
// GPS mySerial Port Arduino Mega 2560 //Tx1 Rx1
HardwareSerial& mySerial = Serial1;
HardwareSerial& LoRaUART = Serial2;
// mySerial Port Arduino UNO,這樣接SoftwareSerial Rx 
//SoftwareSerial mySerial(3, 4); // RX, TX
String readString, servo1, servo2;
void getgps(TinyGPS &gps);
void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ;
  }
  Serial.println("Hardware Serial Ready");

  // set the data rate for the GPS port
  mySerial.begin(9600);
  while (!Serial1) {
    ;
  }
  Serial.println("GPS Serial Ready");
  
  LoRaUART.begin(9600);
  while (!LoRaUART) {
    ;
  }
  Serial.println("LoRa Module Serial Ready");
}

void loop() { // run over and over
  while (mySerial.available()) {
    delay(3);  //delay to allow buffer to fill 
    if (mySerial.available() >0) {
      char c = mySerial.read();
      if(gps.encode(c))      // if there is a new valid sentence...
      {
        getgps(gps);         // then grab the data.
      }
      readString += c; //makes the string readString
    }
  }
  if (readString.length() >0) {
        //Serial.println(readString); //see what was received
        servo1 = readString.substring(0, 6); //get the first four characters
        servo2 = readString.substring(8, 9); //get the next four characters 
        if (servo1 == "$GPRMC") {
          Serial.println(servo1);  //print to serial monitor to see parsed results
          Serial.println(servo2);
          Serial.println("still tracking... ");
        }
      readString="";
  } 
}

void getgps(TinyGPS &gps)
{
  // To get all of the data into varialbes that you can use in your code, 
  // all you need to do is define variables and query the object for the 
  // data. To see the complete list of functions see keywords.txt file in 
  // the TinyGPS and NewSoftSerial libs.
  
  // Define the variables that will be used
  float latitude, longitude;
  //unsigned long latitude_dec, longitude_dec;
  //String latitude_hex, longitude_hex;
  // Then call this function
  gps.f_get_position(&latitude, &longitude);
  // You can now print variables latitude and longitude
  Serial.print("Lat/Lng: "); 
  Serial.print(latitude,5); 
  Serial.print(", "); 
  Serial.println(longitude,5);
  unsigned long latitude_dec = latitude/180*pow(2,32);
  unsigned long longitude_dec = longitude/180*pow(2,32);
  String latitude_hex = String(latitude_dec,HEX);
  String longitude_hex = String(longitude_dec,HEX);
  Serial.println("Lat/Long: " + latitude_hex + "," + longitude_hex);
  Serial.println("Debug AT: at+dtx=22,901aff"+latitude_hex+longitude_hex);
  LoRaUART.println("at+dtx=22,901aff"+latitude_hex+longitude_hex);
  
}
