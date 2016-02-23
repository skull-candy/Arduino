/*Arduino Controlled Voltage Monitor
*/

#include <LiquidCrystal.h>

//Initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

void setup()
{
  lcd.begin(16, 2);                      //Set up the LCD's number of columns and rows
  lcd.print("Internal Votlage");           //First line opening message
  lcd.setCursor(0, 1);
  lcd.print("Adruino Minitor");         //Second line opening message
  delay(5000);
  lcd.setCursor(0, 1);                   //Clear bottom line
  lcd.print("                ");
  lcd.setCursor(0,0);
  lcd.print(" 3v    5v   12v ");          //Update top line readout
}

void loop()
{
  lcd.setCursor(0, 1);
  float f = analogRead(0) * 4.88 / 1023;   // 3.3 => 9.9
  lcd.print(f, 2);                        // print float with two decimals


  lcd.setCursor(6, 1);
  float g = analogRead(1) * 8.5 / 1023;	  // 5.0 => 9.9
  lcd.print(g, 2);

  
  lcd.setCursor(11, 1);
  float h = analogRead(2) * 17.25 / 1023;  // 12.0 => 25.0
  lcd.print(h, 2);

  delay(500);
}
