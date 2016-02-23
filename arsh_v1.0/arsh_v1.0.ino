// Sketch by Ahsan and Arslan about thermaly controling a relay and 
// V.1.2 Date : 12-06-15
#include <LiquidCrystal.h>
#include <String.h>
LiquidCrystal lcd(12, 11, 5, 4, 3, 2); // initialize the library with the numbers of the interface pins
int Area1 = 10;               // Area 1 or motor 1 location area pin location
int Pled = 13;             // Power LED NOT TO BE CHANGED OR USED BY ANY OTHER ITEM
int TEMPSENSOR = 0;       // analog pin for LM 35 Thermal Sensor
int centi = 0;
int RAWTEMPVALUE;
int TMAX = 32;       // Thermal Relay THRESHOLD
int TADJ = 2;       // Thermal Sensor Adjustment
int backlit = 7; 
int backlitv = HIGH; 
int backlitvS = 0;               // S stands for State of backlitv

/// degress sign............####################...............
byte newchar[8]={
  B11100,
  B10100,
  B11100,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
} ;


void setup()
{
  pinMode(backlit, OUTPUT);
pinMode(Area1, OUTPUT);
pinMode(Pled, OUTPUT);
digitalWrite (Pled , HIGH); // digitalWrite (R4 , HIGH); // to turn off the R4 on bootup
digitalWrite (backlit , backlitv);
Serial.begin(9600);  
digitalWrite(Area1, LOW);
// LCD AREA BEGUN..........##############################...................
lcd.begin(16, 2);
  // SPLASH SCREEN START
 lcd.clear() ;
  lcd.setCursor(0,0);
  lcd.write("  I.E.P.E.W.S.  ");
  lcd.setCursor(0,1);
  lcd.write("    VER=1.2    ");
  delay(3000); 
 lcd.clear() ;
 lcd.setCursor(0,0);
  lcd.write("  Project By :  ");
    lcd.setCursor(0,1);
  lcd.write("ARSH. Roll# 2015");
  delay(2000);
  lcd.clear() ;
  
  //SPLASH SCREEN END
  //STATIC DISPLAY MENU STARTS
  lcd.setCursor(0,0);
  lcd.write("TEMP=");
  lcd.setCursor(0,1);
  lcd.write("STATUS=");

  lcd.setCursor(13,0) ;
  lcd.print(TMAX) ;
  
  // degree sign setup code
    lcd.createChar(0,newchar);
  lcd.setCursor(10,0);
  lcd.write((byte)0);
   
    // degree sign setup code
    lcd.createChar(0,newchar);
  lcd.setCursor(15,0);
  lcd.write((byte)0);
  
}

void loop()
{
  
  int lengthOfArr = 10; // Just change the Value for length of input command
    int index = 0;
    char charArr[lengthOfArr];
    String stringCommand = "";
    while(Serial.available() > 0 && index < lengthOfArr)
    {
      char aChar = Serial.read();
      charArr[index] = aChar;
      index++;
      stringCommand = String(charArr);
    }


    if (stringCommand.startsWith("backlitv on") || stringCommand.startsWith("1")){
      if(backlitvS == 0)
      {
        digitalWrite (backlitv ,HIGH);
        Serial.println("backlitv Energized (Console Command)");
        lcd.setCursor(0,1);
        lcd.write("ON");  
        backlitvS = 1;
      }
      else
      {
        Serial.println("backlitv is Already ON");
      }
    }


  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  int value = analogRead(TEMPSENSOR); // taken from arduino cook book 
  Serial.print(value); 
  Serial.print(" > ");
  
 RAWTEMPVALUE = analogRead(TEMPSENSOR);
  float mv = ( RAWTEMPVALUE/1024.0)*5000; 
  float celsius = mv/10;
  float farh = (celsius*9)/5 + 32;
  float centi =(celsius-TADJ);
  Serial.print(centi);
   Serial.println(" degrees Celsius, ");
     lcd.setCursor(5,0);
  lcd.print(centi);
   
 // THERMAL ACTIVE RELAY CODE ENDS...###########################................... 
    if(centi >= TMAX) // ######## || set temp here || ########
  {
    delay(500); // wait for half second
    digitalWrite(Area1, HIGH);
    lcd.setCursor(7,1);
        lcd.write("OVER HEAT");
}
  else
  {
    digitalWrite(Area1, LOW);
    lcd.setCursor(7,1);
        lcd.write("ALL Z WEL");    
  }
// THERMAL ACTIVE RELAY CODE ENDS...
delay(500); // wait for 100 mili-second  
}



