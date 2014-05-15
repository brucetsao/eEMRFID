#include <EEPROM.h>
#include <LiquidCrystal.h>
#include <String.h>
#define openkeypin 4
int debugmode = 0; 
#define relayopendelay 1500 
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
int keycontroladdress = 10;
int keystartaddress = 20;
int Maxkey = 0 ;
String Keylist[100] ;
String keyno1;

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

}

void loop()
{ 
     if  (readTags(&newtag[0])) 
   {
           keyno1 = getcardnumber(&newtag[0]) ;
        Serial.print("Card Number is :(") ;
        Serial.print(keyno1) ;
        Serial.print(")\n") ;
         lcd.setCursor(1,1);
        lcd.print("                     "); 
         lcd.setCursor(1,1);
        lcd.print(getcardnumberA(&newtag[0])); 
         lcd.setCursor(1,2);
        lcd.print("                     "); 
         lcd.setCursor(1,1);
        lcd.print(getcardnumberB(&newtag[0])); 
        
          if (checkAllKey(keyno1) )
            {
              opendoor();
            }
            else
            {
               closedoor() ;
            } 
   }


            
    delay(500);         //延時0.5秒
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
               Keylist[i] = String(readkey(keyarea+(i*20) ) );
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
void decryptkey(String kk)
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
        cardvalue[i] = tmp1 ;
    }
       if (debugmode == 1)
      {
          Serial.println(tmp1,HEX);
          Serial.print("/");
      }
          Serial.print(")");
          Serial.print("\n");
} 

String readkey(int keyarea)
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
        cardvalue[i] = kk ;
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
    return getcardnumber(&cardvalue[0]);
}

void writekey(int keyarea)
{
      int kk,i ;
 for (i = 0; i< 14; i++)
       {
          EEPROM.write(keyarea+i, cardvalue[i]);
       }
   
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
       lcd.print("Access Granted:Open"); 
       Serial.println("Access Granted:Door Open"); 
      delay(relayopendelay) ;
       digitalWrite(openkeypin,LOW);
}

void closedoor()
{
        digitalWrite(openkeypin,LOW);
        lcd.setCursor(0, 3); 
       lcd.print("Access Denied:Closed"); 
       Serial.println("Access Denied:Door Closed"); 
}


