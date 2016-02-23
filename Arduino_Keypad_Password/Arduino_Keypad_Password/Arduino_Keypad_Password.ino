#include <Password.h>
#include <Keypad.h>
#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C  lcd(0x3F, 2, 1, 0, 4, 5, 6, 7); // 0x27 is the I2C bus address for an unmodified backpack
 
Password password = Password("55555");  // password
int dlugosc = 5;                        // length of the password
 
int ledRed = 7;  ////red
int ledGreen = 6; /// green
int buzzer = 12; ///blue
 
int ilosc; // number of clicks
 
const byte ROWS = 4; // rows
const byte COLS = 4; // cols
 
char keys[ROWS][COLS] =
{
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
 
byte rowPins[ROWS] = {2,3,4,5};
byte colPins[COLS] = {11,10,9,8};
 
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );
 
void setup()
{
  Serial.begin(9600);
  keypad.addEventListener(keypadEvent);  
    lcd.begin (20, 4); // for 16 x 2 LCD module
  lcd.setBacklightPin(3, POSITIVE);
  lcd.setBacklight(HIGH);
  pinMode(ledRed, OUTPUT);  
  pinMode(ledGreen, OUTPUT);
  pinMode(buzzer, OUTPUT);
 
  digitalWrite(ledRed, LOW);
  digitalWrite(ledGreen, LOW);
 
  lcd.setCursor(1,0);
  lcd.print("  :Welcome:");
  lcd.setCursor(0,1);
  lcd.print("PLEASE ENTER PIN");
}
 
void loop()
{
  keypad.getKey();
}

void keypadEvent(KeypadEvent eKey)
{
  switch (keypad.getState())
  {
    case PRESSED:
   
int i;
for( i = 1; i <= 1; i++ )
{
  digitalWrite(buzzer, HIGH);  
  delay(50);            
  digitalWrite(buzzer, LOW);  
  delay(50);      
}    
 
Serial.print("Pressed: ");
Serial.println(eKey);
 
switch (eKey)
{
/*
case '#':
break;
 
case '*':
break;
*/
 
default:
ilosc=ilosc+1;
password.append(eKey);
}
//Serial.println(ilosc);
 
if(ilosc == 1)
{
lcd.clear();
lcd.setCursor(1,0);
lcd.print("   < PIN >");
lcd.setCursor(0,1);
lcd.print("*_");
}
if(ilosc == 2)
{
lcd.clear();
lcd.setCursor(1,0);
lcd.print("   < PIN >");
lcd.setCursor(0,1);
lcd.print("**_");
}
if(ilosc == 3)
{
lcd.clear();
lcd.setCursor(1,0);
lcd.print("   < PIN >");
lcd.setCursor(0,1);
lcd.print("***_");
}
if(ilosc == 4)
{
lcd.clear();
lcd.setCursor(1,0);
lcd.print("   < PIN >");
lcd.setCursor(0,1);
lcd.print("****_");
}
if(ilosc == 5)
{
lcd.clear();
lcd.setCursor(1,0);
lcd.print("   < PIN >");
lcd.setCursor(0,1);
lcd.print("*****_");
}
if(ilosc == 6)
{
lcd.clear();
lcd.setCursor(1,0);
lcd.print("   < PIN >");
lcd.setCursor(0,1);
lcd.print("******_");
}
if(ilosc == 7)
{
lcd.clear();
lcd.setCursor(1,0);
lcd.print("   < PIN >");
lcd.setCursor(0,1);
lcd.print("*******_");
}
if(ilosc == 8)
{
lcd.clear();
lcd.setCursor(1,0);
lcd.print("   < PIN >");
lcd.setCursor(0,1);
lcd.print("********");
}
 
if(ilosc == dlugosc)
{
delay(250);
checkPassword();
ilosc = 0;
}
}
}
 
void checkPassword()
{
  if (password.evaluate())
  {
int i;
for( i = 1; i <= 2; i++ )
{
  digitalWrite(buzzer, HIGH);  
  delay(70);            
  digitalWrite(buzzer, LOW);  
  delay(70);      
}    
    ilosc = 0;
 
    Serial.println("Success");    
 
    digitalWrite(ledRed, LOW);
    digitalWrite(ledGreen, HIGH);
 
    lcd.clear();
    lcd.setCursor(0,1);
    lcd.print(" << SUCCESS >>");
     delay(5000);  
        lcd.clear();
    lcd.setCursor(0,0);
        lcd.print("  :Welcome:  ");
    lcd.setCursor(0,1);
    lcd.print("PLEASE ENTER PIN");  
        digitalWrite(ledRed, LOW);
    digitalWrite(ledGreen, LOW);
 }
  else  
  {
int i;
for( i = 1; i <= 1; i++ )
{
  digitalWrite(buzzer, HIGH);  
  delay(200);            
  digitalWrite(buzzer, LOW);  
  delay(200);      
}  
    ilosc = 0;  
    password.reset();
 
    Serial.println("Wrong");
    digitalWrite(ledRed, LOW);
       lcd.setBacklight(HIGH);
      delay(200);    
      digitalWrite(ledRed, HIGH);  
         lcd.setBacklight(LOW);
            delay(200); 
      digitalWrite(ledRed, LOW);  
         lcd.setBacklight(HIGH);
  delay(200);    
      digitalWrite(ledRed, HIGH); 
         lcd.setBacklight(LOW); 
  delay(200);            
  digitalWrite(ledRed, LOW);
     lcd.setBacklight(HIGH);  
  delay(200);  

 
    lcd.clear();
    lcd.setCursor(0,1);
    lcd.print("  << WRONG >>");
    delay(1000);
   digitalWrite(ledRed, LOW);  
    lcd.clear();
    lcd.setCursor(1,0);
    lcd.print("  :Welcome:");
    lcd.setCursor(0,1);
    lcd.print("PLEASE ENTER PIN");    
  }
}
