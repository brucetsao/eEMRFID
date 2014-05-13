#include <SoftwareSerial.h>
#define SIZESTR 12
SoftwareSerial RFID(2, 3);

String msg = String();

void setup()  
{
  Serial.begin(9600);
  Serial.println("[*] Serial Ready !");
  RFID.begin(9600);
  Serial.println("[*] RFID Ready !");
}

char c;

void loop()
{
  while(RFID.available()>0)
  {
    c=RFID.read();
    msg.concat(c);
    if(msg.length()==SIZESTR+1){
      Serial.print("[*] Tag ID = ");
      Serial.println(msg);
      msg="";  
      RFID.flush();
    }

  }

}






















