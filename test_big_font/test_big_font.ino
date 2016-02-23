#include <characters.h>
#include <phi_super_font.h>

/*
Warning: PLEASE DO NOT REMOVE THIS COMMENT WHEN REDISTRIBUTING!
This is an example showcasing the first official release of the Phi-super-font library version 20110501
Please obtain manual of this library from:
http://liudr.wordpress.com/phi_super_font/

Programmed by Dr. John Liu
Revision: 05/01/2011
Free software for educational and personal uses.
No warrantee!
Commercial use without authorization is prohibited.
Find details of the Phi-1 shield, Phi-2 shield, and Phi-super-font or contact Dr. Liu at
http://liudr.wordpress.com/phi-1-shield/
http://liudr.wordpress.com/phi-2-shield/

All rights reserved.
*/

#define phi_2_shield // Define this if you are using the phi 2 shield
//#define phi_1_shield // Define this if you are using the phi 1 shield
#define sparsh_circle

#include <LiquidCrystal.h>
#include <stdio.h>
#include <avr/pgmspace.h>
#include <phi_super_font.h>

#include "defs.h"

LiquidCrystal lcd(LCD_RS,LCD_EN,LCD_D4,LCD_D5,LCD_D6,LCD_D7); // Create the lcd object

byte img1[]={
0,0,0,1,2,2,2,1,0,0,0,1,2,2,2,1,0,0,0,0,
0,0,2,1,0,2,0,1,2,0,2,1,0,0,0,1,2,0,0,0,
0,0,3,0,1,3,1,0,3,1,3,0,1,1,1,0,3,0,0,0,
0,0,0,0,2,2,2,0,0,0,0,0,2,2,2,0,0,0,0,0
};// line 0, 2, 1, 3 order.

boolean inverted=false;

void setup()
{
  lcd.begin(20, 4);
  init_super_font(&lcd); 
}

void loop()
{
  
}
