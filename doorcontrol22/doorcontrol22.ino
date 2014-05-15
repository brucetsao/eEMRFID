#include <EEPROM.h>
#include <LiquidCrystal.h>
#include <String.h>
#include "pitches.h"
#include <Keypad.h>

#define openkeypin 4
int debugmode = 0; 
#define relayopendelay 1500 
#define tonepin 3
/* LiquidCrystal display with:

LiquidCrystal(rs, enable, d4, d5, d6, d7) 
LiquidCrystal(rs, rw, enable, d4, d5, d6, d7) 
LiquidCrystal(rs, enable, d0, d1, d2, d3, d4, d5, d6, d7) 
LiquidCrystal(rs, rw, enable, d0, d1, d2, d3, d4, d5, d6, d7) 
R/W Pin Read = LOW / Write = HIGH   // if No pin connect RW , please leave R/W Pin for Low State

Parameters
*/

LiquidCrystal lcd(8,9,10,38,40,42,44);   //ok
String tag1 = String("0230313034423937343930353803");
String tag2 = String("0231323030323142423938313003");

//int tag1[14] = {2 ,31 ,32 ,30 ,30 ,32 ,31 ,42 ,42 ,39 ,38 ,31 ,30 ,3};
//int tag2[14] = {2 ,30 ,31 ,30 ,34 ,42 ,39 ,37 ,34 ,39 ,30 ,35 ,38 ,3};
byte newtag[14] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0}; // used for read comparisons
byte cardvalue[14] ; 
// this is EEPROM Address  =====
int keycontroladdress = 10;
int keystartaddress = 20;
int keypadcontroladdress = 16;
int keyspadtartaddress = 560;
// this is EEPROM Address  =====
int Maxkey = 0 ;
int PadMaxkey = 0 ;
// this for all key data store in EEPROM
String Keylist[100] ;
String PadKeylist[100] ;
String keyno1;
String padkey1 = String("A123456");

int melody[] = {
   NOTE_C4, NOTE_G3,NOTE_G3, NOTE_A3, NOTE_G3,0, NOTE_B3, NOTE_C4};

// note durations: 4 = quarter note, 8 = eighth note, etc.:
int noteDurations[] = {
   4, 8, 8, 4,4,4,4,4 };
// this for keypad 4*4

const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
//define the cymbols on the buttons of the keypads
char hexaKeys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {33, 35, 37, 39}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {41, 43, 45, 47}; //connect to the column pinouts of the keypad

//initialize an instance of class NewKeypad
//Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 
Keypad customKeypad =  Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 
char customKey ;


void setup()
{
    pinMode(openkeypin,OUTPUT);
    digitalWrite(openkeypin,LOW);
    Serial2.begin(9600);    // start serial to RFID reader
    Serial.begin(9600);  // start serial to PC 
    Serial.println("RFID EM Tags Read");
    lcd.begin(20, 4);
// 設定 LCD 的行列數目 (4 x 20)
   lcd.setCursor(0,0);
  // 列印 "Hello World" 訊息到 LCD 上 
    lcd.print("RFID EM Tags Read"); 
     getAllKey(keycontroladdress,keystartaddress) ;
    getAllPadKey(keypadcontroladdress ,keyspadtartaddress ) ;

}

void loop()
{ 
     if  (readTags(&newtag[0])) 
   {
           keyno1 = getcardnumber(&newtag[0]) ;
        Serial.print("Card Number is :(") ;
        Serial.print(keyno1) ;
        Serial.print(")\n") ;
        LCDdispTag(keyno1);
          if (checkAllKey(keyno1) )
            {
              opendoor();
            }
            else
            {
               closedoor() ;
            } 
   }

    customKey = customKeypad.getKey();  
  if (customKey){
    keypadtone();
      Serial.println(customKey);
        if (customKey == '*')
           {
              chkpadpassword();
           } 
         if (customKey == 'D')  
              {     
                  Serial.print("now enter New Tag Card \n");
                   insertTagKey();
              }
         if (customKey == 'C')  
              {     
                   Serial.print("Now Write all new  password\n") ;
                      writeAllTagKey() ;    
                   getAllKey(keycontroladdress,keystartaddress) ;    
              }
  }    
            
    delay(500);         //延時0.5秒
}

void chkpadpassword()
{
                 Serial.println("enter keypad status");
                keyno1 = getpadkeyin() ;
               Serial.print("keypad is:(");
               Serial.print(keyno1);
               Serial.print(")\n");
           //-------------------------    
          if (checkAllPadKey(keyno1) )
            {
                  digitalWrite(openkeypin,HIGH);
                      lcd.setCursor(0, 3); 
                 lcd.print("Password Granted:Open"); 
                 Serial.println("Password Granted:Door Open"); 
                 passtone();
                delay(relayopendelay) ;
                 digitalWrite(openkeypin,LOW);
            }
            else
            {
               //   digitalWrite(openkeypin,LOW);
                  lcd.setCursor(0, 3); 
                 lcd.print("Password Denied:Closed"); 
                 nopasstone();
                 Serial.println("Password Denied:Door Closed"); 
            }                

        //-------------
}

void insertTagKey()
{
        while (1)
         {
              if (readTags(&newtag[0])) 
                {
                     keyno1 = getcardnumber(&newtag[0]) ;
                      Serial.print("New Card Number is :(") ;
                      Serial.print(keyno1) ;
                      Serial.print(")\n") ;
                      LCDdispTag(keyno1);
              
                        if (  Maxkey  < 100)
                           {
                                Maxkey ++ ;
                                 Keylist[Maxkey-1]   = keyno1 ;
                                Serial.print("Now enter password is :(");
                                Serial.print(Maxkey);
                                Serial.print("/");
                                Serial.print(keyno1) ;
                                Serial.print(")\n");
                                passtone() ;
      
                           }
                return ;
                }           
         }
 }
         
         
String getpadkeyin()
{

  String tmpstr = String("");
   char customKey = customKeypad.getKey();
     lcd.setCursor(0, 2); 
     lcd.print("                     "); 
   lcd.setCursor(0, 2); 
                 
   while (customKey != '#')
   {
     if (customKey){
           keypadtone();
        Serial.println(customKey);
         lcd.print(customKey); 
            tmpstr.concat(customKey);
            } 
       customKey = customKeypad.getKey();
   }
  return tmpstr ;
}


void checkMasterKey(String kk)
{
        if (kk == tag1)
            {
              opendoor();
            }
            else
            {
              closedoor();
            }
  
}

boolean checkAllKey(String kk)
{
  if (debugmode == 1)
     { 
        Serial.print("read for check  key is :(");
        Serial.print(kk);
        Serial.print("/");
        Serial.print(Maxkey);
        Serial.print(")\n");
     }
 int i = 0 ;
  if (Maxkey > 0 )
    for (i = 0 ; i < (Maxkey ) ; i ++)
      {  
          if (debugmode == 1)
             { 
                  Serial.print("Compare internal key value is  :(");
                  Serial.print(i);
                  Serial.print(")");
                  Serial.print(Keylist[i]);
                 Serial.print("/\n");
             }  
        if ( kk == Keylist[i] )
           {
               Serial.println("Card comparee is successful");
               return true ;
           }
      }
    return false ;
}

boolean checkAllPadKey(String kk)
{
  if (debugmode == 1)
     { 
        Serial.print("read for check pad key is :(");
        Serial.print(kk);
        Serial.print("/");
        Serial.print(PadMaxkey);
        Serial.print(")\n");
     }
 int i = 0 ;
  if (PadMaxkey > 0 )
    for (i = 0 ; i < (PadMaxkey ) ; i ++)
      {  
       if (debugmode == 1)
             { 
                  Serial.print("Compare internal passwords value is  :(");
                  Serial.print(i);
                  Serial.print(")");
                  Serial.print(PadKeylist[i]);
                 Serial.print("/\n");
             }  
        if ( kk == PadKeylist[i] )
           {
               Serial.println("Open Password comparee is successful");
               return true ;
           }
      }
    return false ;
}




void getAllKey(int controlarea, int keyarea)
{
    int i = 0;
    Maxkey = getKeyinSizeCount(controlarea) ;
          if (debugmode == 1)
            {
               Serial.print("Max key is :("); 
               Serial.print(Maxkey);
               Serial.print(")\n");
            }
   if ( Maxkey >0)
     {
         for(i = 0 ; i < (Maxkey); i++)
           {
               Keylist[i] = readkey(keyarea+(i*20), &cardvalue[0])  ;
          if (debugmode == 1)
            {              
               Serial.print("inter key is :(");
               Serial.print(i);
               Serial.print("/") ;
               Serial.print(Keylist[i] );
               Serial.print(")\n");
            }  
           }
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




int getKeyinSizeCount(int keycontrol)
{
      if (debugmode == 1)
         { 
             Serial.print("Read memory head is :(") ;
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
              Serial.print("key head is ok \n") ;
              Serial.print("key count is :(") ;
             Serial.print(tmp) ;
             Serial.print(") \n") ;
            }
         return tmp ;  
     }
     else
     {
        if (debugmode == 1)
                   Serial.print("key head is fail \n") ;
        tmp = -1 ;
     }
  // if (val)
  return tmp ;
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



void decryptkey(String kk, byte *dd)
{
  int tmp1,i ;
 if (debugmode == 1)
    {
        Serial.print("decryptkey key : ");
        Serial.print("key1 =");
        Serial.print(kk);
        Serial.print(":(");
    }
    
  for (i = 0 ; i <14; i++)
    {
        tmp1 = unstrzero(kk.substring(i*2, (i+1)*2) ,16);
        *(dd+i) = tmp1 ;
    }
       if (debugmode == 1)
      {
          Serial.println(tmp1,HEX);
          Serial.print("/");
      }
          Serial.print(")");
          Serial.print("\n");
} 

String readkey(int keyarea, byte *dd)
{
    int kk,i ;
      if (debugmode == 1)
      {
          Serial.print("read key : ");
          Serial.print("key1 =(");
      }
      
       for (i = 0; i< 14; i++)
       {
         kk = EEPROM.read(keyarea+i);
        *(dd+i) = kk ;
              if (debugmode == 1)
              {
                  Serial.println(kk,HEX);
                  Serial.print("/");
              }
       }
      if (debugmode == 1)
        {
          Serial.print(")");
          Serial.println("");
        }
    return getcardnumber(dd);
}

void writekey(int keyarea, byte *dd)
{
      int kk,i ;
 for (i = 0; i< 14; i++)
       {
          EEPROM.write(keyarea+i, *(dd+i));
       }
   
}


void writeAllTagKey()
{
  int i = 0 ;
// if (debugmode == 1)
      {
          Serial.print("now write Tag key data is :(");
          Serial.print(keycontroladdress );
          Serial.print("/");
          Serial.print(Maxkey);
          Serial.print(")\n");
      }
        EEPROM.write(keycontroladdress, 100);  //mean activate key store function
      EEPROM.write(keycontroladdress+2, Maxkey);  //mean activate key store function

    for(i = 0 ; i < Maxkey ; i++)
    {
    //   if (debugmode == 1)
   //       {
              Serial.print("now write pad key str is :(");
              Serial.print(i);
              Serial.print("/");
              Serial.print(Keylist[i]);
              Serial.print(")\n");
    //      }
          decryptkey(Keylist[i],&cardvalue[0]) ;
          writekey(keystartaddress +(i*20),&cardvalue[0]);

          //          EEPROM.write(keyarea + i, str.charAt(i)); 
    }    
 
          writeTagKeyHead(keypadcontroladdress,Maxkey );

}

void writeTagKeyHead(int keyarea, int pk)
{
  EEPROM.write(keyarea, 100);  //mean activate key store function
  EEPROM.write(keyarea+2, pk);  //mean activate key store function

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

String getcardnumber(byte *cc)
{
    return joinCardBytes(cc, 14) ;
}
String getcardnumberA(byte *cc)
{
    return joinCardBytes(cc, 7) ;
}

String getcardnumberB(byte *cc)
{
    return joinCardBytes(cc+7, 7) ;
}

String joinCardBytes(byte *cc, int len)
{
  String retstring = String("");
  int i = 0 ;
for (i = 0 ; i < len; i++)
  {
      retstring.concat(strzero(*(cc+i),2,16) );
  }
      return retstring;
}

boolean readTags(byte *data)
{
  boolean temp = false;
  byte data1 ;
  if (Serial2.available() > 0) 
  {
    // read tag numbers
    delay(100); // needed to allow time for the data to come in from the serial buffer.

    for (int z = 0 ; z < 14 ; z++) // read the rest of the tag
    {
      data1 = Serial2.read();
      *(data+z) = data1;
    }
     temp = true ;
   // Serial2.flush(); // stops multiple reads

    // do the tags match up?
   // checkmytags();
  }

  // now do something based on tag type
   return temp ;
}


void opendoor()
{
          digitalWrite(openkeypin,HIGH);
            lcd.setCursor(0, 3); 
          passtone();
       lcd.print("Access Granted:Open"); 
       Serial.println("Access Granted:Door Open"); 
      delay(relayopendelay) ;
       digitalWrite(openkeypin,LOW);
}

void closedoor()
{
        digitalWrite(openkeypin,LOW);
        nopasstone() ;
        lcd.setCursor(0, 3); 
       lcd.print("Access Denied:Closed"); 
       Serial.println("Access Denied:Door Closed"); 
}


void passtone()
{
         tone(tonepin,NOTE_E5 ) ;
         delay(300);
         noTone(tonepin);

}

void nopasstone()
{
  int delaytime = 150 ;
  int i = 0 ;
  for (i = 0 ;i<3;i++)
        {
          tone(tonepin,NOTE_E5,delaytime) ;
    //      tone(tonepin,NOTE_C4,delaytime) ;
       //   tone(tonepin,NOTE_C5,delaytime ) ;
        delay(delaytime);
        }
      noTone(tonepin);
}

void keypadtone()
{
         tone(tonepin,NOTE_E5 ) ;
         delay(130);
         noTone(tonepin);

}

void LCDdispTag(String kk)
{
         lcd.setCursor(1,1);
        lcd.print("                     "); 
         lcd.setCursor(1,1);
        lcd.print(kk.substring(0,14)); 
         lcd.setCursor(1,2);
        lcd.print("                     "); 
         lcd.setCursor(1,2);
        lcd.print(kk.substring(15,28)); 
  
  
}

