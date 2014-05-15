//#include <SoftwareSerial.h>
//SoftwareSerial RFID(2, 3); // RX and TX
#include <LiquidCrystal.h>

/* LiquidCrystal display with:

LiquidCrystal(rs, enable, d4, d5, d6, d7) 
LiquidCrystal(rs, rw, enable, d4, d5, d6, d7) 
LiquidCrystal(rs, enable, d0, d1, d2, d3, d4, d5, d6, d7) 
LiquidCrystal(rs, rw, enable, d0, d1, d2, d3, d4, d5, d6, d7) 
R/W Pin Read = LOW / Write = HIGH   // if No pin connect RW , please leave R/W Pin for Low State

Parameters
*/
LiquidCrystal lcd(8,9,10,38,40,42,44);   //ok

#define openkeypin 4
int debugmode = 0; 
int data1 = 0;
int ok = -1;

int tag1[14] = {2 ,31 ,32 ,30 ,30 ,32 ,31 ,42 ,42 ,39 ,38 ,31 ,30 ,3};
int tag2[14] = {2 ,30 ,31 ,30 ,34 ,42 ,39 ,37 ,34 ,39 ,30 ,35 ,38 ,3};
int newtag[14] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0}; // used for read comparisons
byte cardvalue[14] ; 
void setup()
{
  Serial2.begin(9600);    // start serial to RFID reader
  Serial.begin(9600);  // start serial to PC 
    Serial.println("RFID EM Tags Read");
    lcd.begin(20, 4);
// 設定 LCD 的行列數目 (4 x 20)
   lcd.setCursor(0,0);
  // 列印 "Hello World" 訊息到 LCD 上 
    lcd.print("RFID EM Tags Read"); 
}

void loop()
{
   if  (readTags(&newtag[0])) 
   {
        Serial.print("Card Number is :(") ;
        Serial.print(getcardnumber(&newtag[0])) ;
        Serial.print(")\n") ;
         lcd.setCursor(1,1);
        lcd.print("                     "); 
         lcd.setCursor(1,1);
        lcd.print(getcardnumberA(&newtag[0])); 
         lcd.setCursor(1,2);
        lcd.print("                     "); 
         lcd.setCursor(1,1);
        lcd.print(getcardnumberB(&newtag[0])); 

   }
   delay(400);
}


boolean comparetag(int aa[14], int bb[14])
{
  boolean ff = false;
  int fg = 0;
  for (int cc = 0 ; cc < 14 ; cc++)
  {
    if (aa[cc] == bb[cc])
    {
      fg++;
    }
  }
  if (fg == 14)
  {
    ff = true;
  }
  return ff;
}

void checkmytags() // compares each tag against the tag just read
{
  ok = 0; // this variable helps decision-making,
  // if it is 1 we have a match, zero is a read but no match,
  // -1 is no read attempt made
  if (comparetag(newtag, tag1) == true)
  {
    ok++;
  }
  if (comparetag(newtag, tag2) == true)
  {
    ok++;
  }
}

boolean readTags(int *data)
{
  boolean temp = false;

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

String getcardnumber(int *cc)
{
    return joinCardBytes(cc, 14) ;
}

String getcardnumberA(int *cc)
{
    return joinCardBytes(cc, 7) ;
}

String getcardnumberB(int *cc)
{
    return joinCardBytes(cc+7, 7) ;
}

String joinCardBytes(int *cc, int len)
{
  String retstring = String("");
  int i = 0 ;
for (i = 0 ; i < len; i++)
  {
      retstring.concat(strzero(*(cc+i),2,16) );
  }
      return retstring;
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

