#include <EEPROM.h>

int keypadcontroladdress = 16;
int keyspadtartaddress = 560;
byte cardvalue[10] ; 
int debugmode = 0; 
int Maxkey = 0 ;
int PadMaxkey = 0 ;
String Keylist[100] ;
String PadKeylist[100] ;

void setup() {
  Serial.begin(9600);
  Serial.println("Now Write key data") ;
  // 在 keycontroladdress = 20 上寫入數值100
  if (EEPROM.read(keypadcontroladdress) == 100)
    {
        getAllPadKey(keypadcontroladdress ,keyspadtartaddress ) ;

          Serial.println("padkey data Stored in EEPROM") ;
          Serial.print(PadMaxkey) ;
          Serial.print("padkey(s) Stored in EEPROM") ;
          Serial.print("\n");
          listAllPadKey();
      }
    else
{
          Serial.println("No any pad key data Stored in EEPROM") ;
}  

}
void loop() {
}

void listAllPadKey()
{
    int i = 0;
       for(i = 0 ; i < (PadMaxkey); i++)
       {
         
           Serial.print("Password(");
           Serial.print(i);
           Serial.print("):") ;
           Serial.print(PadKeylist[i] );
           Serial.print("/\n");
        }

  
}
void getAllPadKey(int controlarea, int keyarea)
{
    int i = 0;
    PadMaxkey = getPadKeyinSizeCount(controlarea) ;
          if (debugmode == 1)
            {
               Serial.print("Max key is :("); 
               Serial.print(PadMaxkey);
               Serial.print(")\n");
            }
   if ( PadMaxkey >0)
     {
         for(i = 0 ; i < (PadMaxkey); i++)
           {
               PadKeylist[i] = String(readpadkey(keyarea+(i*10) ) );
          if (debugmode == 1)
            {              
               Serial.print("inter key is :(");
               Serial.print(i);
               Serial.print("/") ;
               Serial.print(PadKeylist[i] );
               Serial.print(")\n");
            }  
           }
     }

}


int getPadKeyinSizeCount(int keycontrol)
{
      if (debugmode == 1)
         { 
             Serial.print("Read Pad key memory head is :(") ;
             Serial.print(keycontrol) ;
             Serial.print("/") ;
             Serial.print(EEPROM.read(keycontrol) ) ;
             Serial.print("/") ;
             Serial.print(EEPROM.read(keycontrol+2) ) ;
             Serial.print(")") ;
             Serial.print("\n") ;
         }
   int tmp = -1;  
   if (EEPROM.read(keycontrol) == 100)
     {
         tmp = EEPROM.read(keycontrol+2) ;
          if (debugmode == 1)
            {
              Serial.print("pad key head is ok \n") ;
              Serial.print("pad key count is :(") ;
             Serial.print(tmp) ;
             Serial.print(") \n") ;
            }
         return tmp ;  
     }
     else
     {
        if (debugmode == 1)
                   Serial.print("pad key head is fail \n") ;
        tmp = -1 ;
     }
  // if (val)
  return tmp ;
}



String readpadkey(int keyarea)
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


