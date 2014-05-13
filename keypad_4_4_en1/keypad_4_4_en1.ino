/* @file Enhance Keypad use
|| @version 1.0
||  Author Bruce modified from keypad library  examples download from http://playground.arduino.cc/Code/Keypad#Download @ keypad,zip 
*/
/* LiquidCrystal display with:

LiquidCrystal(rs, enable, d4, d5, d6, d7) 
LiquidCrystal(rs, rw, enable, d4, d5, d6, d7) 
LiquidCrystal(rs, enable, d0, d1, d2, d3, d4, d5, d6, d7) 
LiquidCrystal(rs, rw, enable, d0, d1, d2, d3, d4, d5, d6, d7) 
R/W Pin Read = LOW / Write = HIGH   // if No pin connect RW , please leave R/W Pin for Low State

*/

#include <Keypad.h>
#include <LiquidCrystal.h>

LiquidCrystal lcd(5,6,7,38,40,42,44);   //ok

const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
//define the cymbols on the buttons of the keypads
char hexaKeys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {23, 25, 27, 29}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {31, 33, 35, 37}; //connect to the column pinouts of the keypad

//initialize an instance of class NewKeypad
//Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 
Keypad customKeypad =  Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 

void setup(){
  Serial.begin(9600);
  Serial.println("program start here");
  Serial.println("start LCM1602");
lcd.begin(16, 2);
// 設定 LCD 的行列數目 (16 x 2)  16　行2　列
 lcd.setCursor(0,0);
  // 列印 "Hello World" 訊息到 LCD 上 
lcd.print("hello, world2!"); 
  Serial.println("hello, world!2");

}
  
void loop(){
  long customKey = getpadnumber();
        // now result is printed on LCD
        lcd.setCursor(1,1);
      lcd.print("key :"); 
       lcd.setCursor(7,1);
      lcd.print(customKey); 
        // now result is printed on Serial COnsole
          Serial.print("in  loop  is :") ;
         Serial.println(customKey);
    delay(200);  

}

long getpadnumber()
{
   const int maxstring = 8;
  char getinputnumber[maxstring] ;
  char InputKeyString = 0x00;
   int  stringpz = 0;
   
  while (stringpz < maxstring)
  {
    InputKeyString = getpadnumberchar();
     if (InputKeyString != 0x00)
     {
       if (InputKeyString != 0x13)
         {
           getinputnumber[stringpz] = InputKeyString ;
           stringpz ++ ;
         }
         else
         {
           break ;
         }
     }
}  
stringpz ++;
getinputnumber[stringpz] = 0x00 ;
return (atol(getinputnumber) );
}

char getpadnumberchar()
{
  char InputKey;
  char checkey = 0x00;  
   
  while (checkey == 0x00)
    {
      InputKey = customKeypad.getKey();
      if (InputKey != 0x00 )
        {
        checkey = cmppadnumberchar(InputKey) ;
//      Serial.print("in  getnumberchar for loop  is :") ;
//          Serial.println(InputKey,HEX) ;
        }
      /*  else
        {
          Serial.print("in  getnumberchar and not if  for loop  is :") ;
          Serial.println(InputKey,HEX) ;
        }
        */
        delay(50);
    }  
  //  Serial.print("exit  getnumberchar is :") ;
  //  Serial.println(checkey,HEX) ;
   return (checkey); 
}


char cmppadnumberchar(char cmpchar)
{
  const int cmpcharsize = 11 ;
char tennumber[cmpcharsize] = {'0','1','2','3','4','5','6','7','8','9','#'} ;
//char retchar = "" ;  
 for(int i  = 0; i< cmpcharsize; i++)
   {
    if (cmpchar == tennumber[i] )
      {
        if (cmpchar == '#')
          {
               return  (0x13) ;
          }
          else
          {
              return  (cmpchar) ;
          }
      }
        
   }
  return  (0x00) ;
}  
