#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x3F, 16, 2);


void setup()
{
   lcd.init();
  lcd.setBacklight(HIGH);
  Serial.begin(9600);
}


void loop()
{
   lcd.setCursor (0, 0);
  lcd.print("  HELLO  WORLD  ");
   lcd.setCursor (0, 1);
  lcd.print("16x2 I2C  MODULE");
  //  delay(1000);
  //  lcd.setBacklight(LOW);      // Backlight off
  //  delay(250);
  //  lcd.setBacklight(HIGH);     // Backlight on
  delay(1000);
}
