#include <avr/wdt.h>
#include <Wire.h>
#include <SodaqDS3231.h>
#include <ClickButton.h>
#include <Wire.h>
#include <SPI.h>
#include <LiquidCrystalI2C.h>
LiquidCrystal_I2C lcd(0x3F, 20, 4);
#include <SimpleTimer.h>
SimpleTimer rtcpoll;
SimpleTimer rtctemp;
SimpleTimer battprocess;
// Nr. of buttons in the array
const int buttons = 3;

// the LED
const int ledPin[buttons] = { 8, 9, 10}; // Arduino pins to the LEDs
int ledState[buttons]     = { 1, 1, 1 };
int LEDfunction[buttons]  = { 0, 0, 0 };
const int R4 = 11 ;
// Arduino input pins from the buttons (these are not in an array for simplicity just now)
const int buttonPin1 = 4;
const int buttonPin2 = 5;
const int buttonPin3 = 6;
int lcdstatus = 0;
// Instantiate ClickButton objects in an array
ClickButton button[3] = {
  ClickButton (buttonPin1, LOW, CLICKBTN_PULLUP),
  ClickButton (buttonPin2, LOW, CLICKBTN_PULLUP),
  ClickButton (buttonPin3, LOW, CLICKBTN_PULLUP),
};

char weekDay[][4] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };

//Analog volt read pin
const int voltPin = A1;
//Variables for voltage divider
float denominator;
int resistor1 = 3300;
int resistor2 = 1000;
 float voltage;
 
// Temperature Related Reading
const int tempInPin = A0;
int tempInValue ; //temperature read
int tempOutDeg ;
int tempadjustment = 45 ;
 

void setup()
{
  wdt_enable(WDTO_8S); ///////////// watchdog
  lcd.init();
  lcd.begin (20, 4); // for 16 x 2 LCD module
  lcd.setBacklight(HIGH);
  lcd.clear();
 // Serial.begin(9600);
  pinMode(R4, OUTPUT);
  digitalWrite(8, HIGH);
  digitalWrite(9, HIGH);
  digitalWrite(10, HIGH);
  digitalWrite(11, HIGH);
  lcdstatus = 1;
  //Convert resistor values to division value
  //  Equation is previously mentions voltage divider equation
  //  R2 / (R1 + R2)
  //  In this case returns 0.20 or 1/5
  denominator = (float)resistor2 / (resistor1 + resistor2);

  {
    Wire.begin();
    rtc.begin();

    //    if (! rtc.isrunning()) {
    //      Serial.println("rtc is NOT running!");
    //      // following line sets the rtc to the date & time this sketch was compiled
    //      rtc.adjust(DateTime(__DATE__, __TIME__));
    //    }

    for (int i = 0; i < buttons; i++)
    {
      pinMode(ledPin[i], OUTPUT);

      // Setup button timers (all in milliseconds / ms)
      // (These are default if not set, but changeable for convenience)
      button[i].debounceTime   = 10;   // Debounce timer in ms
      button[i].multiclickTime = 50;  // Time limit for multi clicks
      button[i].longClickTime  = 1000; // Time until long clicks register
    }
 //   Serial.println("EVERY THING IS OK  AND RUNING !");
  }
  //////////////////////////////////////////TIME CODE ENDS//////////////////////////////////////
  lcd.home (); // set cursor to 0,0
  lcd.print("Bismillaahi   Tawak-");
  lcd.setCursor(0, 1);
  lcd.print("-kaltu Alallahi Wala");
  lcd.setCursor(0, 2);
  lcd.print("Hawla Wa Laa Quwwata");
  lcd.setCursor(0, 3);
  lcd.print("Illaa Billaah -- DUA");
  delay(3000);
  digitalWrite (9, LOW);
  lcd.clear();
  lcd.home (); // set cursor to 0,0
  lcd.print("   SUZUKI  GS 150   ");
  lcd.setCursor(0, 1);
  lcd.print("     RIDE  SAFE     ");
  lcd.setCursor(0, 2);
  lcd.print("V2.0 - DEVELOPED BY  ");
  lcd.setCursor(0, 3);
  lcd.print("AHSAN - 0315 7478785");
  delay(2000);
  lcd.clear();
  digitalWrite(9, HIGH);
  rtcpoll.setInterval(500L, lcdclock);
  rtctemp.setInterval(1000L, inttemp);
  battprocess.setInterval(500L, batt);
}

uint32_t old_ts;

void loop()
{
   wdt_reset(); ///////////// watchdog
  //Obtain RAW voltage data
  voltage = analogRead(voltPin);
  //Convert to actual voltage (0 - 5 Vdc)
  voltage = (voltage / 1024) * 5.1 ;
  //Convert to voltage before divider
  //  Divide by divider = multiply
  //  Divide by 1/5 = multiply by 5
  voltage = voltage / denominator;
  //Output to serial
 


  if (voltage > 10 )
  {
    ////////////////////////////////////////////Button Code Starts/////////////////////////////
    // Go through each button and set the corresponding LED function
    for (int i = 0; i < buttons; i++)
    {
      // Update state of all buitton
      button[i].Update();
      // Save click codes in LEDfunction, as clicks counts are reset at next Update()
      if (button[i].clicks != 0) LEDfunction[i] = button[i].clicks;
      // Simply toggle LED on single clicks
      // (Cant use LEDfunction like the others here,
      //  as it would toggle on and off all the time)
      if (button[i].clicks == 1) ledState[i] = !ledState[i];

      // blink faster if double clicked
      if (LEDfunction[i] == 2) ledState[i] = (millis() / 500) % 2;

      // blink even faster if triple clicked
      if (LEDfunction[i] == 3) ledState[i] = (millis() / 200) % 2;

      // slow blink (must hold down button. 1 second long blinks)
      if (LEDfunction[i] == -1) ledState[i] = (millis() / 1000) % 2;

      // slower blink (must hold down button. 2 second loong blinks)
      if (LEDfunction[i] == -2) ledState[i] = (millis() / 2000) % 2;

      // even slower blink (must hold down button. 3 second looong blinks)
      if (LEDfunction[i] == -3) ledState[i] = (millis() / 3000) % 2;
    }

    // update the LEDs
    for (int i = 0; i < buttons; i++)
    {
      digitalWrite(ledPin[i], ledState[i]);
    }
    if (ledState[0] == 0)
    {
      lcd.setCursor(11, 1);
      lcd.print("R FOG ON ");
    }
    else {
      lcd.setCursor(11, 1);
      lcd.print("R FOG OFF");
    }

    if (ledState[1] == 0)
    {
      lcd.setCursor(0, 2);
      lcd.print("HAZARD ON    ");
      lcd.setBacklight(LOW);
    }
    else {
      lcd.setCursor(0, 2);
      lcd.print("HAZARD OFF  ");
      lcd.setBacklight(HIGH);

    }

    if (ledState[2] == 0)
    {
      lcd.setCursor(0, 1);
      lcd.print("L FOG ON ");
    }
    else {
      lcd.setCursor(0, 1);
      lcd.print("L FOG OFF");

    }
    ////////////////////////////////////////////button code ends///////////////////////////////
  }
  else {
    // lcd.clear();
    digitalWrite(8, HIGH);
    digitalWrite(9, HIGH);
    digitalWrite(10, HIGH);
    digitalWrite(11, HIGH);

    lcd.setCursor(0, 1);
    lcd.print("BAT LOW OR BELOW 10v");

    lcd.setCursor(0, 2);
    lcd.print("ALL DISARMD");


    //lcd.noDisplay();
  }
  
//if (B4 == 1)
//    {
//      lcdstatus = 1;
//         lcd.setCursor(0, 0);
//    lcd.print("LCD TURN OFF REQUEST");
//             lcd.setCursor(0, 1);
//    lcd.print("      RECEIVED      ");
//             lcd.setCursor(0, 2);
//    lcd.print("   OTHER FUNCTIONS  ");
//             lcd.setCursor(0, 3);
//    lcd.print("  REMAIN FUNCTIONAL ");
//    delay(3000);
//    lcd.clear();
//    lcd.noDisplay();
//    }
//    
//if (lcdstatus == 0) {
//  
//  lcd.display();
//      lcd.print("LCD TURN ON  REQUEST");
//             lcd.setCursor(0, 1);
//      lcd.print("      RECEIVED      ");
//             lcd.setCursor(0, 2);
//      lcd.print("    WELCOME  BACK ! ");
//             lcd.setCursor(0, 3);
//      lcd.print("    DRIVE SAFE !!!  ");
//          delay(3000);
//  }
    
  rtcpoll.run();
  rtctemp.run();
  battprocess.run();
}
 void batt()
 {
  
     lcd.setCursor(12, 2);
    lcd.print("B:");
    
  if (voltage < 10)
  {
            lcd.setCursor(14, 1);
    lcd.print("0");
    lcd.setCursor(14, 2);
    lcd.print(voltage , 2);
    lcd.setCursor(19, 2);
    lcd.print("V");

  }
  else {
    lcd.setCursor(14, 2);
    lcd.print(voltage , 2);
    lcd.setCursor(19, 2);
    lcd.print("V");
  }

  
  }
void lcdclock()
{
  DateTime now = rtc.now(); //get the current date-time
  uint32_t ts = now.getEpoch();

  lcd.home (); // set cursor to 0,0
  lcd.setCursor (0, 0);
  if (now.hour() < 10) { // Add a zero, if necessary, as above
    lcd.print(0);
  }
  ////////////////////////////////24 hour to 12 conversion starts//////////////////////////////
  if (now.hour() >= 13) {
    int thour = (now.hour() - 12);
    if (thour < 10) { // Add a zero, if necessary, as above
      lcd.print(0);
    }
    lcd.print(thour);
  }
  else {
    lcd.print(now.hour()); // Display the current hour
  }
  ////////////////////////////////24 hour to 12 conversion ends/////////////////////////////////
  lcd.setCursor (8, 0);
  switch (now.hour()) {
    case  13:
      lcd.print("PM");
      break;
    case 14:
      lcd.print("PM");
      break;
    case 15:
      lcd.print("PM");
      break;
    case 16:
      lcd.print("PM");
      break;
    case 17:
      lcd.print("PM");
      break;
    case 18:
      lcd.print("PM");
      break;
    case 19:
      lcd.print("PM");
      break;
    case 20:
      lcd.print("PM");
      break;
    case 21:
      lcd.print("PM");
      break;
    case 22:
      lcd.print("PM");
      break;
    case 23:
      lcd.print("PM");
      break;
    default:
      lcd.print("AM");
      break;
  }

  lcd.setCursor (2, 0);
  lcd.print(":");
  lcd.setCursor (3, 0);

  if (now.minute() < 10) { // Add a zero, if necessary, as above
    lcd.print(0);
  }
  lcd.print(now.minute(), DEC); // Display the current minutes

  lcd.setCursor (5, 0);
  lcd.print(":");
  lcd.setCursor (6, 0);

  if (now.second() < 10) { // Add a zero, if necessary, as above
    lcd.print(0);
  }
  lcd.print(now.second(), DEC); // Display the current seconds


if  (now.date() < 10){
lcd.setCursor (11, 0);
  lcd.print("0");
  lcd.setCursor (12, 0);
  lcd.print(now.date(), DEC);
} 
else {
lcd.setCursor (11, 0);
  lcd.print(now.date(), DEC);
  }

  lcd.setCursor (13, 0);
  lcd.print("- ");

  lcd.setCursor (14, 0);
  // lcd.print(now.month(), DEC);
  switch (now.month()) {
    case  1:
      lcd.print("JAN");
      break;
    case 2:
      lcd.print("FEB");
      break;
    case 3:
      lcd.print("MAR");
      break;
    case 4:
      lcd.print("APR");
      break;
    case 5:
      lcd.print("MAY");
      break;
    case 6:
      lcd.print("JUN");
      break;
    case 7:
      lcd.print("JUL");
      break;
    case 8:
      lcd.print("AUG");
      break;
    case 9:
      lcd.print("SEP");
      break;
    case 10:
      lcd.print("OCT");
      break;
    case 11:
      lcd.print("NOV");
      break;
    case 12:
      lcd.print("DEC");
      break;

    default:
      lcd.print("!");
      break;
  }

  lcd.setCursor (17, 0);
  lcd.print("-");

  lcd.setCursor (18, 0);
  //  lcd.print(now.year(), DEC);
  switch (now.year()) {
    case  2015:
      lcd.print("15");
      break;
    case 2016:
      lcd.print("16");
      break;
    case 2017:
      lcd.print("17");
      break;
    default:
      lcd.print("!");
      break;
  }
}
void inttemp ()
{
  rtc.convertTemperature();             //convert current temperature into registers
  //  Serial.print(rtc.getTemperature()); //read registers and display the temperature
  //  Serial.println("deg C");
  lcd.setCursor (14, 3);
  lcd.print("CT:");
  lcd.setCursor (17, 3);
  lcd.print(rtc.getTemperature() , 0);
  lcd.setCursor (19, 3);
  lcd.print("C");

  //Read Temperature Sensor
  tempInValue = analogRead(tempInPin);

  tempOutDeg = (5 * tempInValue * 100 / 1024) + tempadjustment;
   lcd.setCursor (7, 3);
  lcd.print("OT:");
  lcd.setCursor(10, 3);
  lcd.print(tempOutDeg);
  lcd.setCursor (12, 3);
  lcd.print("C");

  
 if (((millis() / 1000)/60 ) > 99)
 {
   lcd.setCursor(0, 3);
  lcd.print("T:");
  lcd.setCursor (2, 3);
  lcd.print((millis() / 1000)/60);
     lcd.setCursor(5, 3);
lcd.print(" ");
  }
else {
     lcd.setCursor(0, 3);
  lcd.print("ERT:");
  lcd.setCursor (4, 3);
  lcd.print((millis() / 1000)/60);
  }
}

