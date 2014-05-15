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
  int i = 0 ;
  int k = 0 ;
  if (Serial2.available() > 0) 
  {
      delay(100);
      for (i= 0 ; i < 14; i++)
        {
             k = Serial2.read();
             Serial.print(k, HEX);
             Serial.print(" ");
        }
             Serial.print("\n");
        //    Serial2.flush();
  }
}
