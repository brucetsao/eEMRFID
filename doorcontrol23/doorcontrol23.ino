#include <EEPROM.h>

int keycontroladdress = 10;
int keystartaddress = 20;
String key1 = String("0230313034423937343930353803") ;
String key2 = String("0231323030323142423938313003") ;
byte cardvalue[14] ; 
int debugmode = 0; 
int Maxkey = 0 ;
String Keylist[100] ;
String keyno1;


void setup() {
  int i = 0 ;
  Serial.begin(9600);
  Serial.println("Now read key data in EEPROM") ;
     getAllKey(keycontroladdress,keystartaddress) ; 
   for (i <0 ; i< Maxkey ; i++)
    {
         Serial.print("Key (");
         Serial.print(i);
         Serial.print("/");
         Serial.print(Keylist[i]);
         Serial.print("}");
         Serial.print("\n");
         
       
         }   
     
}
void loop() {
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


