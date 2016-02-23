#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C  lcd(0x3F, 2, 1, 0, 4, 5, 6, 7); // 0x27 is the I2C bus address for an unmodified backpack

 int analogInputAmps = A3;
  
 int readAmpsADC = 0;
 float amps = 0.0;
 
void setup(){
   pinMode(analogInputAmps, INPUT);
  
 lcd.begin (20, 4); // for 16 x 2 LCD module
  lcd.setBacklightPin(3, POSITIVE);
  lcd.setBacklight(HIGH);
  {
    Wire.begin();
    Serial.begin(9600);
  }
   delay(2000);
 }

float fmap(float x, float in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void loop() {
   readAmpsADC = analogRead(analogInputAmps);
   amps = fabs(fmap(readAmpsADC, 0.0, 1023.0, 0.01, 5.0));
   amps = amps * 10;
 lcd.setCursor (0, 1);
  lcd.print("Amps:");

  lcd.setCursor (5, 1);
  lcd.print(amps);

  
   delay(200);

}

