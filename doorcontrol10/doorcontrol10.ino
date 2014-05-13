#include <EEPROM.h>

int keycontroladdress = 10;
int keystartaddress = 20;
String key1 = String("316C1155") ;
String key2 = String("6AE4E616") ;
byte cardvalue[4] ; 
int debugmode = 0; 


void setup() {
  Serial.begin(9600);
  Serial.println("Now Write key data") ;
  // 在 keycontroladdress = 20 上寫入數值100
  EEPROM.write(keycontroladdress, 100);  //mean activate key store function
  EEPROM.write(keycontroladdress+2, 2);  //mean activate key store function
  decryptkey(key1);
  writekey(keystartaddress);
  decryptkey(key2);
  writekey(keystartaddress+10);
  if (EEPROM.read(keycontroladdress) == 100)
    {
          Serial.println("key data Stored in EEPROM") ;
          Serial.print(EEPROM.read(keycontroladdress+2)) ;
          Serial.print("key(s) Stored in EEPROM") ;
          Serial.println("");
          Serial.println("Now read key data") ;
          Serial.print("Key1 :(") ;
          Serial.print(readkey(keystartaddress));
          Serial.println(")") ;
            Serial.print("Key2 :(") ;
          Serial.print(readkey(keystartaddress+10));
          Serial.println(")") ;
    }
    else
{
          Serial.println("No any key data Stored in EEPROM") ;
}  

}
void loop() {
}

void decryptkey(String kk)
{
  int tmp1,tmp2,tmp3,tmp4 ;
  tmp1 = unstrzero(kk.substring(0, 2) ,16);
  tmp2 = unstrzero(kk.substring(2, 4) ,16);
  tmp3 = unstrzero(kk.substring(4, 6) ,16);
  tmp4 = unstrzero(kk.substring(6, 8) ,16);
  cardvalue[0] = tmp1 ;
  cardvalue[1] = tmp2 ;
  cardvalue[2] = tmp3 ;
  cardvalue[3] = tmp4 ;
       if (debugmode == 1)
      {
          Serial.print("decryptkey key : ");
          Serial.print("key1 =");
          Serial.print(kk);
          Serial.print(":(");
          Serial.println(tmp1,HEX);
          Serial.print("/");
          Serial.print(tmp2,HEX);
          Serial.print("/");
          Serial.print(tmp3,HEX);
          Serial.print("/");
          Serial.print(tmp4,HEX);
          Serial.print(")");
          Serial.println("");
      }
} 

String readkey(int keyarea)
{
    int k1,k2,k3,k4 ;
    k1 = EEPROM.read(keyarea);
    k2 = EEPROM.read(keyarea+1);
    k3 = EEPROM.read(keyarea+2);
    k4 = EEPROM.read(keyarea+3);
      if (debugmode == 1)
      {
          Serial.print("read key : ");
          Serial.print("key1 =(");
          Serial.println(k1,HEX);
          Serial.print("/");
          Serial.print(k2,HEX);
          Serial.print("/");
          Serial.print(k3,HEX);
          Serial.print("/");
          Serial.print(k4,HEX);
          Serial.print(")");
          Serial.println("");
      }
    return getcardnumber(k1,k2,k3,k4);
}

void writekey(int keyarea)
{
    EEPROM.write(keyarea, cardvalue[0]);
    EEPROM.write(keyarea+1, cardvalue[1]);
    EEPROM.write(keyarea+2, cardvalue[2]);
    EEPROM.write(keyarea+3, cardvalue[3]);
    
}
String strzero(long num, int len, int base)
{
  String retstring = String("");
  int ln = 1 ;
    int i = 0 ; 
    char tmp[10] ;
    long tmpnum = num ;
    int tmpchr = 0 ;
    char hexcode[]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'} ;
    while (ln <= len)
    {
        tmpchr = (int)(tmpnum % base) ;
        tmp[ln-1] = hexcode[tmpchr] ;
        ln++ ;
         tmpnum = (long)(tmpnum/base) ;
/*
        Serial.print("tran :(");
        Serial.print(ln);
        Serial.print(")/(");
        Serial.print(hexcode[tmpchr] );
        Serial.print(")/(");
        Serial.print(tmpchr);
        Serial.println(")");
        */
        
    }
    for (i = len-1; i >= 0 ; i --)
      {
          retstring.concat(tmp[i]);
      }
    
  return retstring;
}

unsigned long unstrzero(String hexstr, int base)
{
  String chkstring  ;
  int len = hexstr.length() ;
  if (debugmode == 1)
      {
          Serial.print("String ");
          Serial.println(hexstr);
          Serial.print("len:");
          Serial.println(len);
      }
    unsigned int i = 0 ;
    unsigned int tmp = 0 ;
    unsigned int tmp1 = 0 ;
    unsigned long tmpnum = 0 ;
    String hexcode = String("0123456789ABCDEF") ;
    for (i = 0 ; i < (len ) ; i++)
    {
  //     chkstring= hexstr.substring(i,i) ; 
      hexstr.toUpperCase() ;
           tmp = hexstr.charAt(i) ;   // give i th char and return this char
           tmp1 = hexcode.indexOf(tmp) ;
      tmpnum = tmpnum + tmp1* POW(base,(len -i -1) )  ;
 
      if (debugmode == 1)
      {
            Serial.print("char:( ");
          Serial.print(i);
            Serial.print(")/(");
          Serial.print(hexstr);
            Serial.print(")/(");
          Serial.print(tmpnum);
            Serial.print(")/(");
          Serial.print((long)pow(16,(len -i -1)));
            Serial.print(")/(");
          Serial.print(pow(16,(len -i -1) ));
            Serial.print(")/(");
          Serial.print((char)tmp);
            Serial.print(")/(");
          Serial.print(tmp1);
            Serial.print(")");
            Serial.println("");
      }
    }
  return tmpnum;
}

long POW(long num, int expo)
{
  long tmp =1 ;
  if (expo > 0)
  { 
        for(int i = 0 ; i< expo ; i++)
          tmp = tmp * num ;
         return tmp ; 
  } 
  else
  {
   return tmp ; 
  }
}

String getcardnumber(byte c1, byte c2, byte c3, byte c4)
{
   String retstring = String("");
    retstring.concat(strzero(c1,2,16));
    retstring.concat(strzero(c2,2,16));
    retstring.concat(strzero(c3,2,16));
    retstring.concat(strzero(c4,2,16));
    return retstring;
}

