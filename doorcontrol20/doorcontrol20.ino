#include <EEPROM.h>

int keypadcontroladdress = 16;
int keyspadtartaddress = 560;
String padkey1 = String("A123456") ;
String padkey2 = String("B123456") ;
byte cardvalue[10] ; 
int debugmode = 0; 


void setup() {
  Serial.begin(9600);
  Serial.println("Now Write key data") ;
  // 在 keycontroladdress = 20 上寫入數值100
  EEPROM.write(keypadcontroladdress, 100);  //mean activate key store function
  EEPROM.write(keypadcontroladdress+2, 2);  //mean activate key store function
   writekey(keyspadtartaddress,padkey1);
    writekey(keyspadtartaddress+10,padkey2);
  if (EEPROM.read(keypadcontroladdress) == 100)
    {

          Serial.println("padkey data Stored in EEPROM") ;
          Serial.print(EEPROM.read(keypadcontroladdress+2)) ;
          Serial.print("padkey(s) Stored in EEPROM") ;
          Serial.println("");
          Serial.println("Now read pad key data") ;
          Serial.print("Pad Key1 :(") ;
          Serial.print(readkey(keyspadtartaddress));
          Serial.println(")") ;
            Serial.print("Pad Key2 :(") ;
          Serial.print(readkey(keyspadtartaddress+10));
          Serial.println(")") ;
    }
    else
{
          Serial.println("No any pad key data Stored in EEPROM") ;
}  

}
void loop() {
}


String readkey(int keyarea)
{
    String tmpstr = String("");
    char     tmpchr ;
   int i = 0 ;
 if (debugmode == 1)
        {
           Serial.print("now read pad key data is :(");
            Serial.print(keyarea);
            Serial.print(")\n");
        }
    
    
    for(i = 0 ; i < 10 ; i++)
    {
         tmpchr =   EEPROM.read(keyarea + i);
         if (tmpchr != 0)
           {
         if (debugmode == 1)
             {
                  Serial.print("now read pad key str is :(");
                  Serial.print(i);
                  Serial.print("/");
                  Serial.print(tmpchr);
                  Serial.print("/");
                  Serial.print(tmpchr,HEX);
                  Serial.print(")\n");
             }
                tmpstr.concat(tmpchr) ;
           }
           else
           {
             break ;
           }
             
    }    
    
    return tmpstr;
}

void writekey(int keyarea, String str)
{
  int i = 0 ;
 if (debugmode == 1)
      {
          Serial.print("now write pad key data is :(");
          Serial.print(keyarea);
          Serial.print("/");
          Serial.print(str);
          Serial.print(")\n");
      }
      
    for(i = 0 ; i < str.length() ; i++)
    {
       if (debugmode == 1)
          {
              Serial.print("now write pad key str is :(");
              Serial.print(i);
              Serial.print("/");
              Serial.print(str.charAt(i));
              Serial.print(")\n");
          }
          EEPROM.write(keyarea + i, str.charAt(i));
    }    
            EEPROM.write(keyarea + str.length(),NULL);

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

