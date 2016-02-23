#include <FastIO.h>
#include <I2CIO.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>
#include <LiquidCrystal_SR.h>
#include <LiquidCrystal_SR2W.h>
#include <LiquidCrystal_SR3W.h>

/*

  Created by Govinda (William) Kalfelz (c) 2013 - www.Wusik.com
  Tested with the Arduino 0022 IDE
  
*/

#include <Time.h>
#include <Wire.h>
#include <DS1307RTC.h>  // a basic DS1307 library that returns time as a time_t
#include <LiquidCrystal.h>

// ----------------------------------------------------------------------------------------------------------------------------------
typedef unsigned char uint8_t;
typedef int int16_t;
typedef unsigned int uint16_t;
typedef long int32_t;
typedef unsigned long uint32_t;

// ----------------------------------------------------------------------------------------------------------------------------------
#define IS_INIT_EEPROM 0 // Use this to init the EEPROM to the default value //
#define SHOWFREEMEM  0   // Shows how much RAM left there is
#define DEVICEADDRESS   0x50 // 1307 RTC device address - 0b0101 0 a2 a1 a0
#define MSG01 3600
#define MSG02 3750
#define MSG03 3900
#define MSG04 3950
#define MSG05 4000

// ----------------------------------------------------------------------------------------------------------------------------------
#if IS_INIT_EEPROM
void setup() 
{  
  pinMode(9, OUTPUT); 
  Wire.begin(); 
  for (int x=0; x<4096; x++) { 
    WireEepromWriteByte(x, 0); 
  }
                   //12345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901345678901234567890 (150) Max
  saveMessageEEPROM("Set Alarm: DD/MM HH:MM T SMTWTFS MESSAGE (up to 20 chars - T is type / 0=AutoDelete 1=Recurring 2=RecurringSimple 3=Reminder - Date as 00/00 = Daily)", MSG01);
  saveMessageEEPROM("Select an Option: (1) Set Clock | (2) List Alarms | (3) Set Alarm | (4) Delete Alarm | (5) Order Agenda", MSG02);
  saveMessageEEPROM("Set New Clock: 00:00 dd/mm/yy", MSG03);
  saveMessageEEPROM("Delete Alarm: ##", MSG04);
  saveMessageEEPROM("There are no more free space", MSG05);
} 

// ----------------------------------------------------------------------------------------------------------------------------------
void loop() { 
  buzzer(); 
  delay(10000); 
}

// ----------------------------------------------------------------------------------------------------------------------------------
void saveMessageEEPROM(char* msg, int pos)
{
  int xp = 0;
  while (1)
  {
    WireEepromWriteByte(pos+xp, msg[xp]);
    xp++;
    if (msg[xp] == 0) break;
  }
  WireEepromWriteByte(pos+xp, 0);
}
#else

// ----------------------------------------------------------------------------------------------------------------------------------
#define TRIGGER_PIN  A3  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     A2  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 100 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.
LiquidCrystal lcd(12, 11, 2, 3, 4, 5);
uint8_t curSeconds = 250;
uint8_t prevMinutes = 250;
uint8_t prevHour = 250;
uint8_t startAlarm = 0;
uint8_t doAlarm = 0;
char charPingSeconds = '.';
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.
uint8_t pingCounter = 0;
uint8_t listAgendaPos = 0;
uint8_t LcdCursor[8] = { B00000,    B01000,    B01100,    B01110,    B01100,    B01000,    B00000,    B00000 };
char scrollMessage[34] = {0};
uint8_t scrollMessagePos = 0;
#define EVENTSIZE 27
#define weekDateMonthLine 2
#define scrollAlarmLine 3

// ----------------------------------------------------------------------------------------------------------------------------------
void setup()
{  
  Wire.begin();
  lcd.begin(20, 4);
  init_big_font();
  lcd.createChar(0, LcdCursor);
  setSyncProvider(RTC.get);
  Serial.begin(9600);
  showMenu();
  pinMode(9, OUTPUT); // Buzzer // 

#if SHOWFREEMEM
  extern unsigned int __data_start;
  extern unsigned int __data_end;
  extern unsigned int __bss_start;
  extern unsigned int __bss_end;
  extern unsigned int __heap_start;
  extern void *__brkval;

  int free_memory;
  if((int)__brkval == 0) free_memory = ((int)&free_memory) - ((int)&__bss_end);
  else free_memory = ((int)&free_memory) - ((int)__brkval);
  Serial.println(free_memory, DEC);
#endif
}

// ----------------------------------------------------------------------------------------------------------------------------------
void loop() 
{ 
  if (Serial.available() > 0) 
  {
    // get incoming byte:
    char inByte = Serial.read();

    if (inByte == '1')
    {
      serialNewLine();
      readMessageEEPROM(MSG03);
      waitInput(14);     
      setRTClock(readNumberSerial(1), readNumberSerial(1), readNumberSerial(1), readNumberSerial(1), ((2000+readNumberSerial(1))-1970));
      serialNewLine();
    }
    else if (inByte == '2')
    {
      listAgendaAlarms();
    }
    else if (inByte == '3')
    {
      serialNewLine();
      readMessageEEPROM(MSG01);
      waitInput(23);

      char nAlarm = findFreeAlarmSpot();
      if (nAlarm == -1)
      {
        readMessageEEPROM(MSG05);
        readAllSerial();
        return;
      }

      WireEepromWriteByte(4+(nAlarm*EVENTSIZE), readNumberSerial(1));
      WireEepromWriteByte(5+(nAlarm*EVENTSIZE), readNumberSerial(1));
      WireEepromWriteByte(0+(nAlarm*EVENTSIZE), readNumberSerial(1));
      WireEepromWriteByte(1+(nAlarm*EVENTSIZE), readNumberSerial(1));

      WireEepromWriteByte(2+(nAlarm*EVENTSIZE), (Serial.read()-'0')); 
      Serial.read();

      char weekD = 0;
      for (uint8_t w=7; w>=1; w--)
      {
        if (Serial.read() == '1') bitSet(weekD, w);
      }
      WireEepromWriteByte(3+(nAlarm*EVENTSIZE), weekD);     
      Serial.read();

      char q = 0;
      while (Serial.available() > 0)
      {
        WireEepromWriteByte(6+q+(nAlarm*EVENTSIZE), Serial.read());
        q++;
        if (q == 20)
        {
          readAllSerial();
          break;
        }
      }
      WireEepromWriteByte(6+q+(nAlarm*EVENTSIZE), 0);

      serialNewLine();
      listAgendaAlarms();
    }
    else if (inByte == '4')
    {
      listAgendaAlarms();
      serialNewLine();
      readMessageEEPROM(MSG04);
      waitInput(2);
      WireEepromWriteByte(3+(readNumberSerial(0)*EVENTSIZE), 0);
      serialNewLine(); 
    }
    else if (inByte == '5')
    {
      organizeAgenda();
    }

    showMenu();
  }

  // ------------------ CLOCK STUFF  ------------------ //
  setSyncProvider(RTC.get);

  if (curSeconds != second())
  {
    curSeconds = second();

    if (listAgendaPos == 0)
    {
      if (prevHour != hour()) lcd.clear();

      pingSeconds();

      if (prevMinutes != minute())
      {
        prevMinutes = minute();

        if (prevHour != hour())
        {
          prevHour = hour();

          // Date //
          char showMSG[15] = {0};
          sprintf(showMSG, "%s %02d/%02d",dayStr(weekday()),day(),month());
          lcd.setCursor(20-strlen(showMSG), weekDateMonthLine);
          lcdPrintString(showMSG);
        }

        render_big_number2(hourFormat12(),0,0,1);
        render_big_number2(minute(),10,0,0);
        if (isAM()) { 
          lcd.setCursor(18, 1);
          lcdPrintString("am");
        } 
        else {
          lcd.setCursor(18, 1);
          lcdPrintString("pm");
        }

        // Check Alarms //
        for (uint8_t qx=0; qx<50; qx++)
        {
          doAlarm = 0;

          if (WireEepromRead(3+(qx*EVENTSIZE)) != 0)
          {
            if (WireEepromRead(4+(qx*EVENTSIZE)) == 0)
            {
              // Every Day Event //
              if (hour() == WireEepromRead((qx*EVENTSIZE)) && minute() == WireEepromRead(1+(qx*EVENTSIZE)) && bitRead(WireEepromRead(3+(qx*EVENTSIZE)),weekday()) == 1) doAlarm = 1;
            }
            else
            {
              // Fixed Day Event //
              if (day() == WireEepromRead(4+(qx*EVENTSIZE)) && month() == WireEepromRead(5+(qx*EVENTSIZE)) && hour() == WireEepromRead((qx*EVENTSIZE)) && minute() == WireEepromRead(1+(qx*EVENTSIZE)) && bitRead(WireEepromRead(3+(qx*EVENTSIZE)),weekday()) == 1) doAlarm = 1;
            }
          }

          if (doAlarm)
          {
            if (WireEepromRead(2+(qx*EVENTSIZE)) == 0 || WireEepromRead(2+(qx*EVENTSIZE)) == 3) WireEepromWriteByte(3+(qx*EVENTSIZE), 0); // DELETE - Not Recurring (or just a reminder)

            clearLCDline(scrollAlarmLine);
            lcd.setCursor(0, scrollAlarmLine);
            for (uint8_t qq=0; qq<20; qq++)
            {
              if (WireEepromRead(6+qq+(qx*EVENTSIZE)) == 0) break;
              lcd.write(WireEepromRead(6+qq+(qx*EVENTSIZE)));
            }

            startAlarm = 1;
            if (WireEepromRead(2+(qx*EVENTSIZE)) == 2) startAlarm = 18; // Simple Recurring
          }
          else
          {
            getNextAgendaEvent();
          }
        }      
      }
    }

    // Now do the Alarm Sound //
    if (startAlarm > 0)
    {
      startAlarm++;
      if (startAlarm < 20) buzzer();
      if (startAlarm > 56) 
      { 
        if (listAgendaPos > 0) prevHour = curSeconds = prevMinutes = 250; 
        else clearLCDline(scrollAlarmLine);
        startAlarm = listAgendaPos = 0;
      }
    }
  }

  // ----------------- //
  pingCounter++;
  if (pingCounter > 254)
  {
    if (startAlarm == 0)
    {
      lcd.setCursor(0, scrollAlarmLine);
      uint8_t ww = scrollMessagePos;
      for (uint8_t w=0; w<20; w++)
      {
        lcd.write(scrollMessage[ww]);
        ww++;
        if (ww >= 32) ww  = 0;
      }
      scrollMessagePos++;
      if (scrollMessagePos >= 32) scrollMessagePos = 0;
    }
    
    pingCounter = 0;
    int cm = sonar.ping_cm(); // Send out the ping, get the results in centimeters.
    if (cm > 0 && cm < 12)
    {
      // Click //
      if (listAgendaPos == 0 && startAlarm > 0)
      {
        startAlarm = 57;
      }
      else
      {
        startAlarm = 30;
        listAgendaLCD();
        waitOneSecond();
      }
    }
  }
}

// ----------------------------------------------------------------------------------------------------------------------------------
void pingSeconds()
{
  lcd.setCursor(8, 0); lcd.write(charPingSeconds);
  lcd.setCursor(8, 1); lcd.write(charPingSeconds);
  if (charPingSeconds == '.') charPingSeconds = ' '; else charPingSeconds = '.';
}

// ----------------------------------------------------------------------------------------------------------------------------------
void clearLCDline(char line)
{
  lcd.setCursor(0, line);
  for (uint8_t qq=0; qq<20; qq++) lcd.write(' '); 
}

// ----------------------------------------------------------------------------------------------------------------------------------
void setRTClock(char hour, char minutes, char day, char month, char year)
{ 
  setSyncProvider(RTC.get);
  time_t t = now();
  tmElements_t tm;
  breakTime(t, tm);
  tm.Hour = (uint8_t) hour;
  tm.Minute = (uint8_t)minutes;
  tm.Day = (uint8_t) day;
  tm.Month = (uint8_t) month;
  tm.Year = (uint8_t) year;
  DS1307RTC::set(makeTime(tm));
}

// ----------------------------------------------------------------------------------------------------------------------------------
void printDigitsLCD(int digits)
{
  lcd.write(':');
  if(digits < 10) lcd.write('0');
  lcd.write(digits);
}

// ----------------------------------------------------------------------------------------------------------------------------------
void waitInput(char nChars)
{
  while (Serial.available() < nChars) { 
    ; 
  } 
}

// ----------------------------------------------------------------------------------------------------------------------------------
void showMenu()
{
  readMessageEEPROM(MSG02);
}

// ----------------------------------------------------------------------------------------------------------------------------------
/*
  
 EEPROM 4096 * 8 (32K)
 
 // Agenda Structure in EEPROM //
 // Alarms
 uint8_t startHour;
 uint8_t startMinute;
 uint8_t isRecurring;
 uint8_t whatWeekDay; // Bits SundayMonday... 11111110
 uint8_t Day;
 uint8_t Month;
 char message[21];
 Size = 27 Bytes
 
 */

// ----------------------------------------------------------------------------------------------------------------------------------
void serialNewLine()
{
  Serial.write('\n');
}

// ----------------------------------------------------------------------------------------------------------------------------------
void printWeekDays(uint8_t weekD)
{
  if (bitRead(weekD, 7) == 1) Serial.write('S');   else Serial.write('_');
  if (bitRead(weekD, 6) == 1) Serial.write('M');   else Serial.write('_');
  if (bitRead(weekD, 5) == 1) Serial.write('T');   else Serial.write('_');
  if (bitRead(weekD, 4) == 1) Serial.write('W');   else Serial.write('_');
  if (bitRead(weekD, 3) == 1) Serial.write('T');   else Serial.write('_');
  if (bitRead(weekD, 2) == 1) Serial.write('F');   else Serial.write('_');
  if (bitRead(weekD, 1) == 1) Serial.write('S');   else Serial.write('_');
}

// ----------------------------------------------------------------------------------------------------------------------------------
uint8_t readNumberSerial(char skipLineAfter)
{
  char tempN = (Serial.read()-'0')*10;
  tempN += Serial.read()-'0';

  if (skipLineAfter == 1) Serial.read();

  return tempN;
}

// ----------------------------------------------------------------------------------------------------------------------------------
char findFreeAlarmSpot()
{
  for (uint8_t x=0; x<50; x++)
  {
    if (WireEepromRead(3+(x*EVENTSIZE)) == 0) return x;
  }

  return -1;  
}

// ----------------------------------------------------------------------------------------------------------------------------------
void readAllSerial()
{
  while (Serial.available() > 0) Serial.read();
}

// ----------------------------------------------------------------------------------------------------------------------------------
void readMessageEEPROM(int pos)
{
  for (uint8_t q=0; q<250; q++)
  {
    if (WireEepromRead(pos+q) == 0) break;
    Serial.write(WireEepromRead(pos+q));
  }
  serialNewLine();
}

// ----------------------------------------------------------------------------------------------------------------------------------
void printSerialSpaces(char spaces)
{
  for (uint8_t s=0; s<spaces; s++) Serial.write(' ');
}

// ----------------------------------------------------------------------------------------------------------------------------------
void listAgendaAlarms()
{
  serialNewLine();
  for (uint8_t x=0; x<50; x++)
  {
    if (WireEepromRead(3+(x*EVENTSIZE)) != 0)
    {
      serialPrintNumber(x);
      Serial.write('#'); printSerialSpaces(1);

      if (WireEepromRead(4+(x*EVENTSIZE)) > 0) 
      {
        serialPrintNumber(WireEepromRead(4+(x*EVENTSIZE)));
        Serial.write('/');
        serialPrintNumber(WireEepromRead(5+(x*EVENTSIZE)));
        printSerialSpaces(1);
      }
      else
      {
        printSerialSpaces(6);
      }

      serialPrintNumber(WireEepromRead((x*EVENTSIZE)));
      Serial.write(':');
      serialPrintNumber(WireEepromRead(1+(x*EVENTSIZE)));
      printSerialSpaces(1);

      if (WireEepromRead(2+(x*EVENTSIZE)) == 0) serialPrintString("Ad"); 
      else if (WireEepromRead(2+(x*EVENTSIZE)) == 1) serialPrintString("R "); 
      else if (WireEepromRead(2+(x*EVENTSIZE)) == 2) serialPrintString("Rs");
      else if (WireEepromRead(2+(x*EVENTSIZE)) == 3) serialPrintString("Rm");
      printSerialSpaces(1);

      printWeekDays(WireEepromRead(3+(x*EVENTSIZE)));
      printSerialSpaces(1); serialPrintString("-"); printSerialSpaces(1);

      for (uint8_t xx=0; xx<20; xx++) 
      {
        if (WireEepromRead(6+xx+(x*EVENTSIZE)) == 0) break;
        Serial.write(WireEepromRead(6+xx+(x*EVENTSIZE)));
      }
      serialNewLine();
    }
  }

  serialNewLine();
}

// ----------------------------------------------------------------------------------------------------------------------------------
void organizeAgenda()
{
  // Clear Temp Area First //
  serialNewLine();
  Serial.write('*');
  for (uint16_t x=(50*EVENTSIZE); x<((50+50)*EVENTSIZE); x++) WireEepromWriteByte(x,0);

  // Do the Daily Alarms First //
  char hour = 0;
  char minutes = 0;
  char pos = 0;
  while (1)
  {
    for (uint8_t x=0; x<50; x++)
    {
      if (WireEepromRead(3+(x*EVENTSIZE)) != 0 && WireEepromRead(4+(x*EVENTSIZE)) == 0 && WireEepromRead(0+(x*EVENTSIZE)) == hour && WireEepromRead(1+(x*EVENTSIZE)) == minutes)
      {
        for (uint8_t q=0; q<EVENTSIZE; q++) WireEepromWriteByte((50*EVENTSIZE)+(pos*EVENTSIZE)+q,WireEepromRead(q+(x*EVENTSIZE)));
        pos++;
      }
    }

    minutes++;
    if (minutes > 60)
    {
      hour++;
      Serial.write('.');
      if (hour > 24) break;
      minutes = 0;
    }
  }

  // Now do the Fixed Dates //
  char day = 1;
  char month = 1;
  while (1)
  {
    for (uint8_t x=0; x<50; x++)
    {
      if (WireEepromRead(3+(x*EVENTSIZE)) != 0 && WireEepromRead(4+(x*EVENTSIZE)) == day && WireEepromRead(5+(x*EVENTSIZE)) == month)
      {
        for (uint8_t q=0; q<EVENTSIZE; q++) WireEepromWriteByte((50*EVENTSIZE)+(pos*EVENTSIZE)+q,WireEepromRead(q+(x*EVENTSIZE)));
        pos++;
      }
    }

    day++;
    if (day > 31)
    {
      month++;
      Serial.write('.');
      if (month > 12) break;
      day = 1;
    }
  }

  Serial.write('*'); Serial.write('\n');
  // Copy Temp Area //
  for (uint16_t x=0; x<(50*EVENTSIZE); x++) WireEepromWriteByte(x,WireEepromRead((50*EVENTSIZE)+x));
  listAgendaAlarms();
}

// ----------------------------------------------------------------------------------------------------------------------------------
void getNextAgendaEvent()
{
  memset(scrollMessage,0,sizeof(scrollMessage));
  for (uint8_t x=listAgendaPos; x<50; x++)
  {
    if (WireEepromRead(3+(x*EVENTSIZE)) != 0 && WireEepromRead(4+(x*EVENTSIZE)) != 0 && WireEepromRead(2+(x*EVENTSIZE)) != 3)
    {
      sprintf(scrollMessage, "%02d/%02d %02d:%02d                      ",WireEepromRead(4+(x*EVENTSIZE)), WireEepromRead(5+(x*EVENTSIZE)), WireEepromRead((x*EVENTSIZE)), WireEepromRead(1+(x*EVENTSIZE)));

      uint8_t xx = 0;
      for (xx=0; xx<20; xx++)
      {
        if (WireEepromRead(6+xx+(x*EVENTSIZE)) == 0) break;
        scrollMessage[12+xx] = WireEepromRead(6+xx+(x*EVENTSIZE));
      }
      break;
    }
  }
}

void listAgendaLCD()
// ----------------------------------------------------------------------------------------------------------------------------------
{
  char listY = 0;
  char hasSomething = 0;
  lcd.clear();

  for (uint8_t x=listAgendaPos; x<50; x++)
  {
    if (WireEepromRead(3+(x*EVENTSIZE)) != 0 && WireEepromRead(4+(x*EVENTSIZE)) != 0 && WireEepromRead(2+(x*EVENTSIZE)) != 3)
    {
      lcd.setCursor(0, listY);
      lcd.write(0);
      lcd.write(' ');
      lcdPrintNumber(WireEepromRead(4+(x*EVENTSIZE)));
      lcd.write('/');
      lcdPrintNumber(WireEepromRead(5+(x*EVENTSIZE)));
      lcd.write(' ');

      lcdPrintNumber(WireEepromRead((x*EVENTSIZE)));
      lcd.write(':');
      lcdPrintNumber(WireEepromRead(1+(x*EVENTSIZE)));

      lcd.setCursor(0, listY+1);
      for (uint8_t xx=0; xx<20; xx++) 
      {
        if (WireEepromRead(6+xx+(x*EVENTSIZE)) == 0) break;
        lcd.write(WireEepromRead(6+xx+(x*EVENTSIZE)));
      }

      hasSomething = 1;
      listAgendaPos = x+1;
      listY += 2;
      if (listY >= 4) break;
    }
  }

  if (hasSomething == 0) 
  { 
    lcd.setCursor(4, 2); 
    lcdPrintString("Fim da Lista"); // End of the list //
    waitOneSecond();
    startAlarm = 56;
  }
}

// ----------------------------------------------------------------------------------------------------------------------------------
void wireStart(uint16_t theMemoryAddress)
{
  Wire.beginTransmission(DEVICEADDRESS);
  Wire.send((int)((theMemoryAddress >> 8) & 0xFF));
  Wire.send((int)((theMemoryAddress >> 0) & 0xFF));
}

// ----------------------------------------------------------------------------------------------------------------------------------
uint8_t WireEepromRead(uint16_t theMemoryAddress) 
{
  wireStart(theMemoryAddress);
  Wire.endTransmission();
  Wire.requestFrom(DEVICEADDRESS, 1);
  return Wire.receive();
}

// ----------------------------------------------------------------------------------------------------------------------------------
void lcdPrintString(char* string)
{
  uint8_t p = 0;
  while (string[p] != 0) { 
    lcd.write(string[p]); 
    p++; 
  }
}

// ----------------------------------------------------------------------------------------------------------------------------------
void lcdPrintNumber(uint8_t number)
{
  lcd.write('0'+(number/10));
  lcd.write('0'+(number-((number/10)*10)));
}

// ----------------------------------------------------------------------------------------------------------------------------------
void serialPrintString(char* string)
{
  uint8_t p = 0;
  while (string[p] != 0) { 
    Serial.write(string[p]); 
    p++; 
  }
}

// ----------------------------------------------------------------------------------------------------------------------------------
void serialPrintNumber(uint8_t number)
{
  Serial.write('0'+(number/10));
  Serial.write('0'+(number-((number/10)*10)));
}
#endif

// ----------------------------------------------------------------------------------------------------------------------------------
void waitOneSecond()
{
  delay(900);
}

// ----------------------------------------------------------------------------------------------------------------------------------
void WireEepromWriteByte(uint16_t theMemoryAddress, uint8_t u8Byte) 
{
  wireStart(theMemoryAddress);
  Wire.send(u8Byte);
  Wire.endTransmission();
  delay(10);
}

// ----------------------------------------------------------------------------------------------------------------------------------
void buzzer()
{
  for (uint8_t x=0; x<100; x++) { digitalWrite(9, HIGH); delayMicroseconds(100+(x/10)); digitalWrite(9, LOW);  delay(1); }
}

