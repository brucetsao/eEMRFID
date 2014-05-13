#include <SoftwareSerial.h>
//SoftwareSerial RFID(3, 2); // RX and TX

int i;

void setup()
{
  Serial2.begin(9600);    // start serial to RFID reader
  Serial.begin(9600);  // start serial to PC 
}

void loop()
{
  if (Serial2.available() > 0) 
  {
     i = Serial2.read();
     Serial.print(i, HEX);
     Serial.print(" ");
  }
}
