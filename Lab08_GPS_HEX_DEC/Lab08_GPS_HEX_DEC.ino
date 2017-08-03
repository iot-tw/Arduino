void setup() {
  float latitude, longitude;
  // put your setup code here, to run once:
  Serial.begin(9600);
  latitude = 179.999998;
  longitude = 54.321123;
  Serial.println(latitude,6);
  Serial.println(longitude,6);
  unsigned long latitude_dec = latitude/180*pow(2,32);
  unsigned long longitude_dec = longitude/180*pow(2,32);
  
  String lat_longitude_hex = String(latitude_dec,HEX) + String(longitude_dec,HEX);
  Serial.println("HEX:" + lat_longitude_hex);
  

}

void loop() {
  // put your main code here, to run repeatedly:

}
