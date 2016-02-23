/*
* TimeRTC.pde
* example code illustrating Time library with Real Time Clock.
*
*/

#include <Time.h> 
#include <Wire.h> 
#include <DS1307RTC.h>  // a basic DS1307 library that returns time as a time_t
#include <LiquidCrystal.h> 
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
void setup()  {
  lcd.begin(16, 2);
 
 

  Serial.begin(9600);
  setSyncProvider(RTC.get);   // the function to get the time from the RTC
  if(timeStatus()!= timeSet)
     Serial.println("Unable to sync with the RTC");
  else
     Serial.println("RTC has set the system time");
    
}

void loop()
{
   digitalClockDisplay(); 
   delay(1000);
}

void digitalClockDisplay(){
  // digital clock display of the time
  Serial.print(hour());
  printDigits(minute());
  printDigits(second());
  Serial.print(" ");
  Serial.print(day());
  Serial.print(" ");
  Serial.print(month());
  Serial.print(" ");
  Serial.print(year());
  Serial.println();
 
lcd.setCursor(2, 0);                                            // Set LCD cursor position (column, row)
lcd.print(hour());
lcd.print(":");
lcd.print (minute());
lcd.print(" ");
lcd.print(second());
lcd.print(" ");
lcd.print("Uhr");
lcd.setCursor(3, 1);                                           // Set LCD cursor position (column,row) 
lcd.print(day());
lcd.print(".");
lcd.print(month());
lcd.print(".");
lcd.print(year());

// Print text to LCD
                                     // Delay to read text
                               // Clear the display

}

void printDigits(int digits){
  // utility function for digital clock display: prints preceding colon and leading 0
  Serial.print(":");
  if(digits < 10)
    Serial.print('0');
  Serial.print(digits);
}
