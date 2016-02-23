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

PROGMEM prog_char msg_00[]="LCD big fonts";
PROGMEM prog_char msg_01[]="Dr.Liu 03/19/11";
PROGMEM prog_char msg_02[]="http://liudr.";
PROGMEM prog_char msg_03[]="wordpress.com";

//Programming custom characters. The custom characters will be lost when power is cut.
#define prog_char_now
PROGMEM prog_char lcd_ch0[]={64,64,64,64,64,64,64,64,0};// 0
PROGMEM prog_char lcd_ch1[]={64,64,64,64,64,31,31,31,0};//1
PROGMEM prog_char lcd_ch2[]={64,64,64,31,31,64,64,64,0};//2
PROGMEM prog_char lcd_ch3[]={64,64,64,31,31,64,31,31,0};//3
PROGMEM prog_char lcd_ch4[]={31,31,31,64,64,64,64,64,0};//4
PROGMEM prog_char lcd_ch5[]={31,31,64,64,64,31,31,31,0};//5
PROGMEM prog_char lcd_ch6[]={31,31,64,31,31,64,64,0};//6
PROGMEM prog_char lcd_ch7[]={31,31,64,31,31,64,31,31,0};//7
PROGMEM const char *ch_item[] = {lcd_ch0, lcd_ch1, lcd_ch2, lcd_ch3, lcd_ch4, lcd_ch5, lcd_ch6, lcd_ch7};

PROGMEM prog_char dow_00[]="SUN";
PROGMEM prog_char dow_01[]="MON";
PROGMEM prog_char dow_02[]="TUE";
PROGMEM prog_char dow_03[]="WED";
PROGMEM prog_char dow_04[]="THU";
PROGMEM prog_char dow_05[]="FRI";
PROGMEM prog_char dow_06[]="SAT";
PROGMEM const char *dow_item[] = {dow_00, dow_01, dow_02, dow_03, dow_04, dow_05, dow_06};

PROGMEM prog_char month_00[]="JAN";
PROGMEM prog_char month_01[]="FEB";
PROGMEM prog_char month_02[]="MAR";
PROGMEM prog_char month_03[]="APR";
PROGMEM prog_char month_04[]="MAY";
PROGMEM prog_char month_05[]="JUN";
PROGMEM prog_char month_06[]="JUL";
PROGMEM prog_char month_07[]="AUG";
PROGMEM prog_char month_08[]="SEP";
PROGMEM prog_char month_09[]="OCT";
PROGMEM prog_char month_10[]="NOV";
PROGMEM prog_char month_11[]="DEC";
PROGMEM const char *month_item[] = {month_00, month_01, month_02, month_03, month_04, month_05, month_06, month_07, month_08, month_09, month_10, month_11};

