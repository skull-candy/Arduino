//**************************************************************************************************
//                                 BIG FONT (2-line) LCD CHARACTERS 
//                                   Adrian Jones, February 2015
//
//Initial idea by http://www.instructables.com/id/Custom-Large-Font-For-16x2-LCDs/step5/Arduino-Sketch/
//**************************************************************************************************

// Build 1
//   r1 150214 - initial build with glyphs and big font character table in program memory

//********************************************************************************************
#define build 1
#define revision 1
//********************************************************************************************

#include <avr/pgmspace.h>            // for memory storage in program space

#include <Wire.h> 
#include <LCD.h>                     // Standard lcd library
#include <LiquidCrystal_I2C.h>       // library for I@C interface

#define I2C_ADDR  0x3F               // address found from I2C scanner
#define RS_pin    0                  // pin configuration for LCM1602 interface module
#define RW_pin    1
#define EN_pin    2
#define BACK_pin  3
#define D4_pin    4
#define D5_pin    5
#define D6_pin    6
#define D7_pin    7

LiquidCrystal_I2C lcd(I2C_ADDR, EN_pin, RW_pin, RS_pin, D4_pin, D5_pin, D6_pin, D7_pin, BACK_pin, POSITIVE);

const char custom[][8] PROGMEM = {                        // Custom character definitions
      { 0x1F, 0x1F, 0x1F, 0x00, 0x00, 0x00, 0x00, 0x00 }, // char 1 
      { 0x18, 0x1C, 0x1E, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F }, // char 2 
      { 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x0F, 0x07, 0x03 }, // char 3 
      { 0x00, 0x00, 0x00, 0x00, 0x00, 0x1F, 0x1F, 0x1F }, // char 4 
      { 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1E, 0x1C, 0x18 }, // char 5 
      { 0x1F, 0x1F, 0x1F, 0x00, 0x00, 0x00, 0x1F, 0x1F }, // char 6 
      { 0x1F, 0x00, 0x00, 0x00, 0x00, 0x1F, 0x1F, 0x1F }, // char 7 
      { 0x03, 0x07, 0x0F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F }  // char 8 
};

// BIG FONT Character Set
// - Each character coded as 1-4 byte sets {top_row(0), top_row(1)... bot_row(0), bot_row(0)..."}
// - number of bytes terminated with 0x00; Character is made from [number_of_bytes/2] wide, 2 high
// - codes are: 0x01-0x08 => custom characters, 0x20 => space, 0xFF => black square

const char bigChars[][8] PROGMEM = {
      { 0x20, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, // Space
      { 0xFF, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, // !
      { 0x05, 0x05, 0x20, 0x20, 0x00, 0x00, 0x00, 0x00 }, // "
      { 0x04, 0xFF, 0x04, 0xFF, 0x04, 0x01, 0xFF, 0x01 }, // #
      { 0x08, 0xFF, 0x06, 0x07, 0xFF, 0x05, 0x00, 0x00 }, // $
      { 0x01, 0x20, 0x04, 0x01, 0x04, 0x01, 0x20, 0x04 }, // %
      { 0x08, 0x06, 0x02, 0x20, 0x03, 0x07, 0x02, 0x04 }, // &
      { 0x05, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, // '
      { 0x08, 0x01, 0x03, 0x04, 0x00, 0x00, 0x00, 0x00 }, // (
      { 0x01, 0x02, 0x04, 0x05, 0x00, 0x00, 0x00, 0x00 }, // )
      { 0x01, 0x04, 0x04, 0x01, 0x04, 0x01, 0x01, 0x04 }, // *
      { 0x04, 0xFF, 0x04, 0x01, 0xFF, 0x01, 0x00, 0x00 }, // +
      { 0x20, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, // 
      { 0x04, 0x04, 0x04, 0x20, 0x20, 0x20, 0x00, 0x00 }, // -
      { 0x20, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, // .
      { 0x20, 0x20, 0x04, 0x01, 0x04, 0x01, 0x20, 0x20 }, // /
      { 0x08, 0x01, 0x02, 0x03, 0x04, 0x05, 0x00, 0x00 }, // 0
      { 0x01, 0x02, 0x20, 0x04, 0xFF, 0x04, 0x00, 0x00 }, // 1
      { 0x06, 0x06, 0x02, 0xFF, 0x07, 0x07, 0x00, 0x00 }, // 2
      { 0x01, 0x06, 0x02, 0x04, 0x07, 0x05, 0x00, 0x00 }, // 3
      { 0x03, 0x04, 0xFF, 0x20, 0x20, 0xFF, 0x00, 0x00 }, // 4
      { 0xFF, 0x06, 0x06, 0x07, 0x07, 0x05, 0x00, 0x00 }, // 5
      { 0x08, 0x06, 0x06, 0x03, 0x07, 0x05, 0x00, 0x00 }, // 6
      { 0x01, 0x01, 0x02, 0x20, 0x08, 0x20, 0x00, 0x00 }, // 7
      { 0x08, 0x06, 0x02, 0x03, 0x07, 0x05, 0x00, 0x00 }, // 8
      { 0x08, 0x06, 0x02, 0x07, 0x07, 0x05, 0x00, 0x00 }, // 9
      { 0xA5, 0xA5, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, // :
      { 0x04, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, // ;
      { 0x20, 0x04, 0x01, 0x01, 0x01, 0x04, 0x00, 0x00 }, // <
      { 0x04, 0x04, 0x04, 0x01, 0x01, 0x01, 0x00, 0x00 }, // =
      { 0x01, 0x04, 0x20, 0x04, 0x01, 0x01, 0x00, 0x00 }, // >
      { 0x01, 0x06, 0x02, 0x20, 0x07, 0x20, 0x00, 0x00 }, // ?
      { 0x08, 0x06, 0x02, 0x03, 0x04, 0x04, 0x00, 0x00 }, // @
      { 0x08, 0x06, 0x02, 0xFF, 0x20, 0xFF, 0x00, 0x00 }, // A
      { 0xFF, 0x06, 0x05, 0xFF, 0x07, 0x02, 0x00, 0x00 }, // B
      { 0x08, 0x01, 0x01, 0x03, 0x04, 0x04, 0x00, 0x00 }, // C
      { 0xFF, 0x01, 0x02, 0xFF, 0x04, 0x05, 0x00, 0x00 }, // D
      { 0xFF, 0x06, 0x06, 0xFF, 0x07, 0x07, 0x00, 0x00 }, // E
      { 0xFF, 0x06, 0x06, 0xFF, 0x20, 0x20, 0x00, 0x00 }, // F
      { 0x08, 0x01, 0x01, 0x03, 0x04, 0x02, 0x00, 0x00 }, // G
      { 0xFF, 0x04, 0xFF, 0xFF, 0x20, 0xFF, 0x00, 0x00 }, // H
      { 0x01, 0xFF, 0x01, 0x04, 0xFF, 0x04, 0x00, 0x00 }, // I
      { 0x20, 0x20, 0xFF, 0x04, 0x04, 0x05, 0x00, 0x00 }, // J
      { 0xFF, 0x04, 0x05, 0xFF, 0x20, 0x02, 0x00, 0x00 }, // K
      { 0xFF, 0x20, 0x20, 0xFF, 0x04, 0x04, 0x00, 0x00 }, // L
      { 0x08, 0x03, 0x05, 0x02, 0xFF, 0x20, 0x20, 0xFF }, // M
      { 0xFF, 0x02, 0x20, 0xFF, 0xFF, 0x20, 0x03, 0xFF }, // N
      { 0x08, 0x01, 0x02, 0x03, 0x04, 0x05, 0x00, 0x00 }, // 0
      { 0x08, 0x06, 0x02, 0xFF, 0x20, 0x20, 0x00, 0x00 }, // P
      { 0x08, 0x01, 0x02, 0x20, 0x03, 0x04, 0xFF, 0x04 }, // Q
      { 0xFF, 0x06, 0x02, 0xFF, 0x20, 0x02, 0x00, 0x00 }, // R
      { 0x08, 0x06, 0x06, 0x07, 0x07, 0x05, 0x00, 0x00 }, // S
      { 0x01, 0xFF, 0x01, 0x20, 0xFF, 0x20, 0x00, 0x00 }, // T
      { 0xFF, 0x20, 0xFF, 0x03, 0x04, 0x05, 0x00, 0x00 }, // U
      { 0x03, 0x20, 0x20, 0x05, 0x20, 0x02, 0x08, 0x20 }, // V
      { 0xFF, 0x20, 0x20, 0xFF, 0x03, 0x08, 0x02, 0x05 }, // W
      { 0x03, 0x04, 0x05, 0x08, 0x20, 0x02, 0x00, 0x00 }, // X
      { 0x03, 0x04, 0x05, 0x20, 0xFF, 0x20, 0x00, 0x00 }, // Y
      { 0x01, 0x06, 0x05, 0x08, 0x07, 0x04, 0x00, 0x00 }, // Z
      { 0xFF, 0x01, 0xFF, 0x04, 0x00, 0x00, 0x00, 0x00 }, // [
      { 0x01, 0x04, 0x20, 0x20, 0x20, 0x20, 0x01, 0x04 }, // Backslash
      { 0x01, 0xFF, 0x04, 0xFF, 0x00, 0x00, 0x00, 0x00 }, // ]
      { 0x08, 0x02, 0x20, 0x20, 0x00, 0x00, 0x00, 0x00 }, // ^
      { 0x20, 0x20, 0x20, 0x04, 0x04, 0x04, 0x00, 0x00 }  // _
};
byte col,row,nb=0,bc=0;                                   // general
byte bb[8];                                               // byte buffer for reading from PROGMEM


//*****************************************************************************************//
//                                      Initial Setup
//*****************************************************************************************//
void setup() {
  lcd.begin(20, 4);
  for (nb=0; nb<8; nb++ ) {                     // create 8 custom characters
    for (bc=0; bc<8; bc++) bb[bc]= pgm_read_byte( &custom[nb][bc] );
    lcd.createChar ( nb+1, bb );
  }
  lcd.clear();
  writeBigString("ALI ALI", 0, 0);
  lcd.setCursor(0, 3); 
  lcd.print(F(" ALI ")); 
  lcd.print(build); 
  lcd.print(F(".")); 
  lcd.print(revision);
  lcd.print(F(" RAM: "));  
  lcd.print(freeRam()); 
  lcd.print(F("B]")); 
  delay(5000);
  lcd.clear();
}

//*****************************************************************************************//
//                                      MAIN LOOP
//*****************************************************************************************//
void loop() {

  lcd.setCursor(0, 0); 
  lcd.print(F(" BIG CHARACTER FONT ")); 
  writeBigString("10:23:46", 0, 2);
}
  
// ********************************************************************************** //
//                                      SUBROUTINES
// ********************************************************************************** //
// writeBigChar: writes big character 'ch' to column x, row y; returns number of columns used by 'ch'
int writeBigChar(char ch, byte x, byte y) {
  if (ch < ' ' || ch > '_') return 0;               // If outside table range, do nothing
  nb=0;                                             // character byte counter 
  for (bc=0; bc<8; bc++) {                        
    bb[bc] = pgm_read_byte( &bigChars[ch-' '][bc] );  // read 8 bytes from PROGMEM
    if(bb[bc] != 0) nb++;
  }  
 
  bc=0;
  for (row = y; row < y+2; row++) {
    for (col = x; col < x+nb/2; col++ ) {
      lcd.setCursor(col, row);                      // move to position
      lcd.write(bb[bc++]);                          // write byte and increment to next
    }
//    lcd.setCursor(col, row);
//    lcd.write(' ');                                 // Write ' ' between letters
  }
  return nb/2-1;                                      // returns number of columns used by char
}

// writeBigString: writes out each letter of string
void writeBigString(char *str, byte x, byte y) {
  char c;
  while ((c = *str++))
  x += writeBigChar(c, x, y) + 1;
}


// ********************************************************************************** //
//                                      OPERATION ROUTINES
// ********************************************************************************** //
// FREERAM: Returns the number of bytes currently free in RAM  
int freeRam(void) {
  extern int  __bss_end, *__brkval; 
  int free_memory; 
  if((int)__brkval == 0) {
    free_memory = ((int)&free_memory) - ((int)&__bss_end); 
  } 
  else {
    free_memory = ((int)&free_memory) - ((int)__brkval); 
  }
  return free_memory; 
}
