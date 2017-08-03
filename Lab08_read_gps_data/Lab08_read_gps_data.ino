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
SoftwareSerial mySerial(10, 11); // RX, TX
String readString, servo1, servo2;
void getgps(TinyGPS &gps);
void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ;
  }
  Serial.println("Hardware Serial Ready");

  // set the data rate for the SoftwareSerial port
  mySerial.begin(9600);
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
  // Then call this function
  gps.f_get_position(&latitude, &longitude);
  // You can now print variables latitude and longitude
  Serial.print("Lat/Long: "); 
  Serial.print(latitude,5); 
  Serial.print(", "); 
  Serial.println(longitude,5);
  
  // Same goes for date and time
  int year;
  byte month, day, hour, minute, second, hundredths;
  gps.crack_datetime(&year,&month,&day,&hour,&minute,&second,&hundredths);
  // Print data and time
  Serial.print("Date: "); Serial.print(month, DEC); Serial.print("/"); 
  Serial.print(day, DEC); Serial.print("/"); Serial.print(year);
  Serial.print("  Time: "); Serial.print(hour, DEC); Serial.print(":"); 
  Serial.print(minute, DEC); Serial.print(":"); Serial.print(second, DEC); 
  Serial.print("."); Serial.println(hundredths, DEC);
  //Since month, day, hour, minute, second, and hundr
  
  // Here you can print the altitude and course values directly since 
  // there is only one value for the function
  Serial.print("Altitude (meters): "); Serial.println(gps.f_altitude());  
  // Same goes for course
  Serial.print("Course (degrees): "); Serial.println(gps.f_course()); 
  // And same goes for speed
  Serial.print("Speed(kmph): "); Serial.println(gps.f_speed_kmph());
  Serial.println();
  
  // Here you can print statistics on the sentences.
  unsigned long chars;
  unsigned short sentences, failed_checksum;
  gps.stats(&chars, &sentences, &failed_checksum);
  //Serial.print("Failed Checksums: ");Serial.print(failed_checksum);
  //Serial.println(); Serial.println();
}
