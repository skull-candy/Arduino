#include <LiquidCrystal.h>
#include <String.h>
// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
int IR_LED = 6;
int R1 = 10;
int R2 = 9;
int R3 = 8;
int R4 = 7;
int Pled = 13;
const int inPin = 0; // analog pin
float sinVal;  
int R1S = 0; // S stands for State of R
int R2S = 0;
int R3S = 0;
int R4S = 0;
void setup()
{  
  Serial.begin(9600);
  pinMode(R1, OUTPUT);
  pinMode(R2, OUTPUT);
  pinMode(R3, OUTPUT);
  pinMode(R4, OUTPUT);
lcd.begin(16, 2);
  lcd.setCursor(0,0);
  lcd.write("  Welcome Sir! ");
  delay(2000);
lcd.clear() ;
  lcd.setCursor(0,0);
  lcd.write("R1");
  lcd.setCursor(0,1);
  lcd.write("DF");
  lcd.setCursor(2,0);
  lcd.write("R2");
  lcd.setCursor(2,1);
  lcd.write("ON");
  lcd.setCursor(4,0);
  lcd.write("R3");
  lcd.setCursor(4,1);
  lcd.write("ON");
  lcd.setCursor(6,0);
  lcd.write("R4");
 lcd.setCursor(6,1);
  lcd.write("ON");
 lcd.setCursor(8,0);
  lcd.write("R5");
 lcd.setCursor(8,1);
  lcd.write("ON");
 lcd.setCursor(11,0);
  lcd.write("Temp:");
}
void loop()
{
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
   
    
    if (stringCommand.startsWith("r1 on") || stringCommand.startsWith("1")){
      if(R1S == 0)
      {
        digitalWrite (R1 ,HIGH);
        Serial.println("R1 Energized (Console Command)");
        lcd.setCursor(0,1);
        lcd.write("ON");  
        R1S = 1;
      }
      else
      {
        Serial.println("R1 is Already ON");
      }
    }
    
      if (stringCommand.startsWith("r1 off") || stringCommand.startsWith("2")){
        if(R1S == 1)
        {  
          digitalWrite (R1 ,LOW);
          Serial.println("R1 De-Energized (Console Command)");
          lcd.setCursor(0,1);
          lcd.write("OF");     
          R1S = 0;
        }
        else
        {
          Serial.println("R1 is Already OFF");
        }
      }
    
    
      if (stringCommand.startsWith("3")){
        digitalWrite (R2 ,HIGH);
        //digitalWrite(12,HIGH);
        Serial.println("R2 Energized (Console Command)");    
      }
    
    
      if (stringCommand.startsWith("4")){
        digitalWrite (R2 ,LOW);
        digitalWrite(12,LOW);
        Serial.println("R2 De-Energized (Console Command)");    
      }
    
  }
  int value = analogRead(inPin); // taken from arduino cook book 
  Serial.print(value); 
  Serial.print(" > ");
  float millivolts = (value / 1024.0) * 5000;
  // sensor output is 10mV per degree Kelvin, 0 Celsius is 273.15
  float celsius = (millivolts / 10) - 273.15 ;
  Serial.print(celsius);
  Serial.println(" degrees Celsius, ");
  lcd.setCursor(11,1);
  lcd.print(celsius);
  delay(200); // wait for one second
  if(celsius >= 50) // ################################# || set temp here || ##########################
  {
    digitalWrite(R4, LOW);
  }
  else
  {
    digitalWrite(R4, HIGH);
    //delay(500); // wait for one second
  }
}

