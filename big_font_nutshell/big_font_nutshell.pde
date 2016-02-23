/*
This is a nutshell release for big fonts for phi-menu
Programmed by Dr. John Liu
Revision: 03/21/2011
Free software for educational and personal uses.
No warrantee!
Commercial use without authorization is prohibited.
Find details of the Phi-1 shield, Phi-2 shield or contact Dr. Liu at
http://liudr.wordpress.com/phi-1-shield/
http://liudr.wordpress.com/phi-2-shield/
All rights reserved.
*/

#define phi_1_shield
//#define phi_2_shield

#include <LiquidCrystal.h>
#include <stdio.h>
#include <avr/pgmspace.h>
#include "defs.h"
#include "progmem.h"
LiquidCrystal lcd(LCD_RS,LCD_EN,LCD_D4,LCD_D5,LCD_D6,LCD_D7); // Create the lcd object

byte font_35_09[]={1,2,3,7,1,6, 1,3,0,0,7,0, 2,2,3,7,5,5, 2,2,3,5,5,7, 3,0,3,4,4,7, 3,2,2,5,5,7, 3,2,2,7,5,7, 2,2,3,0,0,7, 3,2,3,7,5,7, 3,2,3,5,5,7};

byte font_35_AZ[]={3,2,3,7,4,7, 3,2,3,7,5,3, 3,2,2,7,1,1, 3,2,1,7,1,6, 3,2,2,7,5,5, 3,2,2,7,4,4, 3,2,2,7,1,7, 3,0,3,7,4,7, 2,3,2,1,7,1, 2,2,3,3,1,7, 3,0,3,7,4,5, 3,0,0,7,1,1, 3,1,3,7,4,7, 3,2,3,7,0,7, 3,2,3,7,1,7, 3,2,3,7,4,4, 3,2,3,4,4,7, 3,2,3,7,4,3, 3,2,2,5,5,7, 2,3,2,0,7,0, 3,0,3,7,1,7, 3,0,3,6,1,6, 3,0,3,7,6,7, 3,0,3,3,4,3, 3,0,3,4,7,4, 2,2,3,3,5,1};

byte font_35_az[]={2,2,1,2,5,7, 3,1,0,7,1,6, 1,2,2,6,1,1, 0,1,3,6,1,7, 1,2,1,6,5,5, 0,1,2,4,7,4, 1,2,1,1,5,6, 3,1,0,7,0,7, 0,2,0,0,7,0, 0,0,2,3,1,6, 3,0,1,7,4,3, 3,0,0,6,1,1, 3,3,1,7,4,7, 3,2,1,7,0,7, 1,2,1,6,1,6, 3,2,1,7,4,0, 1,2,3,0,4,7, 1,2,2,7,0,0, 1,2,2,1,5,2, 1,3,1,0,6,1, 3,0,3,6,1,7, 3,0,3,6,1,6, 3,0,3,6,7,7, 3,0,3,3,4,3, 3,0,3,1,5,6, 2,2,3,3,5,1};

void setup()
{
  byte ch_buffer[10]; // This buffer is required for custom characters on the LCD.
  lcd.begin(16, 2);
  for (int i=0;i<8;i++)
  {
    strcpy_P((char*)ch_buffer,(char*)pgm_read_word(&(ch_item[i])));
    lcd.createChar(i, ch_buffer);
  }
  show_credit();
}

void loop()
{
  demo();
}

void demo() //Asks you for password for a given number of trials
{
  char msg[4];
  lcd.clear();
  render_big_msg("Big");
  delay(1000);
  lcd.clear();
  render_big_msg("Font");
  delay(1000);
  
  lcd.clear();
  for (byte i=0;i<7;i++)
  {
    big_msg_lcd(dow_00+i*4);
    lcd.setCursor(13,0);
    msg_lcd(dow_00+i*4);
    delay(1000);
  }
  
  for (byte i=0;i<12;i++)
  {
    big_msg_lcd(month_00+i*4);
    lcd.setCursor(13,0);
    msg_lcd(month_00+i*4);
    delay(1000);
  }

  lcd.clear();
  for (byte i=0;i<101;i++)
  {
    lcd.setCursor(0,0);
    render_big_number(i);
    delay(500-i*4);
  }
  
  lcd.clear();
  render_big_msg("Big");
  delay(1000);
  lcd.clear();
  render_big_msg("0TO9");
  delay(1000);
  
  lcd.clear();
  for (byte i='0';i<='9';i++)
  {
    lcd.setCursor(0,0);
    render_big_char(i,0);
    lcd.setCursor(8,0);
    lcd.write(i);
    delay(500);
  }
  
  lcd.clear();
  render_big_msg("Big");
  delay(1000);
  lcd.clear();
  render_big_msg("ATOZ");
  delay(1000);
  
  lcd.clear();
  for (byte i='A';i<='Z';i++)
  {
    lcd.setCursor(0,0);
    render_big_char(i,0);
    lcd.setCursor(8,0);
    lcd.write(i);
    delay(500);
  }

  lcd.clear();
  render_big_msg("Big");
  delay(1000);
  lcd.clear();
  render_big_msg("atoz");
  delay(1000);
  
  lcd.clear();
  for (byte i='a';i<='z';i++)
  {
    lcd.setCursor(0,0);
    render_big_char(i,0);
    lcd.setCursor(8,0);
    lcd.write(i);
    delay(500);
  }
}

//Utility functions
void msg_lcd(char* msg_line)
{
  char msg_buffer[17];
  strcpy_P(msg_buffer,msg_line); 
  lcd.print(msg_buffer);
}

void render_big_char_top(char ch)
{
  if ((ch>='0')&&(ch<='9'))
  {
    for (byte i=0; i<3; i++)
    lcd.write(font_35_09[(ch-'0')*6+i]);
  }
  else if ((ch>='A')&&(ch<='Z'))
  {
    for (byte i=0; i<3; i++)
    lcd.write(font_35_AZ[(ch-'A')*6+i]);
  }
  else if ((ch>='a')&&(ch<='z'))
  {
    for (byte i=0; i<3; i++)
    lcd.write(font_35_az[(ch-'a')*6+i]);
  }
}

void render_big_char_bot(char ch)
{
  if ((ch>='0')&&(ch<='9'))
  {
    for (byte i=0; i<3; i++)
    lcd.write(font_35_09[(ch-'0')*6+i+3]);
  }
  else if ((ch>='A')&&(ch<='Z'))
  {
    for (byte i=0; i<3; i++)
    lcd.write(font_35_AZ[(ch-'A')*6+i+3]);
  }
  else if ((ch>='a')&&(ch<='z'))
  {
    for (byte i=0; i<3; i++)
    lcd.write(font_35_az[(ch-'a')*6+i+3]);
  }
}

void render_big_char(char ch, byte loc)
{
  lcd.setCursor(4*loc,0);
  render_big_char_top(ch);
  lcd.setCursor(4*loc,1);
  render_big_char_bot(ch);
}

void render_big_msg(char msg[])
{
  byte i=0;
  while(msg[i])
  {
    render_big_char(msg[i],i);
    i++;
  }
}

void render_big_number(int number)
{
  char msg[11];
  sprintf(msg,"%d",number);
  render_big_msg(msg);
}

//Display strings stored in PROGMEM. Provide the string name stored in PROGMEM to be displayed on the LCD's current cursor position.
void big_msg_lcd(char* msg_line)
{
  char msg_buffer[17];
  strcpy_P(msg_buffer,msg_line);
  msg_buffer[4]=0; 
  render_big_msg(msg_buffer);
}

void show_credit()
{
  // Display credits
  lcd.clear();
  msg_lcd(msg_00);
  lcd.setCursor(0,1);
  msg_lcd(msg_01);
  delay(2000);
  lcd.clear();
  msg_lcd(msg_02);
  lcd.setCursor(0,1);
  msg_lcd(msg_03);
  delay(2000);
  lcd.clear();
}
