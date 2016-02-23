////////////////////////////////////////////////////////////////////////
//HOME AUTOMATION USING ETHERNET AND ARDUNIO
////////////////////////////////////////////////////////////////////////
//
//Intro:
//This will switch on and off outputs trough your mobile device.
//  Switch On or Off the outputs on startup - .
//  Enable/Disable the All on/off buttons - .
//  Read Temperature and act accordingly .
//  Save/Load statuses from eeprom to keep latest status after power cut .

#include <EthernetUdp.h>
#include <Ethernet.h>
#include <SPI.h>
#include <EEPROM.h>
#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C  lcd(0x3F, 2, 1, 0, 4, 5, 6, 7); // 0x27 is the I2C bus address for an unmodified backpack
#include "RTClib.h"
RTC_DS1307 RTC;

const int Button = 33; //Define button pin
const int Button2 = 37; //Define button pin
const int Button3 = 31; //Define button pin



unsigned int localPort = 8888; //Local port to listen for UDP Packets
IPAddress timeServer(192, 43, 244, 18);//NTP Server IP (time.nist.gov)
const int NTP_PACKET_SIZE= 48;  //NTP Time stamp is in the firth 48 bytes of the message
byte packetBuffer[ NTP_PACKET_SIZE];  //Buffer to hold incomming and outgoing packets
EthernetUDP Udp;  //UDP Instance to let us send and recieve packets
unsigned long epoch; //Unix Epoch time (NTP or RTC depending on state)

////////////////////////////////////////////////////////////////////////
//CONFIGURATION
////////////////////////////////////////////////////////////////////////

//IP manual settings
byte ip[] = {
  192, 168, 0, 2
};   //Manual setup only
byte gateway[] = {
  192, 168, 0, 1
}; //Manual setup only
byte subnet[] = {
  255, 255, 255, 0
}; //Manual setup only

// if need to change the MAC address (Very Rare)
byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};

//Ethernet Port
EthernetServer server = EthernetServer(80); //default html port 80

//The number of outputs going to be switched.
int outputQuantity = 4;  //should not exceed 10

//Invert the output of the leds
boolean outputInverted = true; //true or false
// This is done in case the relay board triggers the relay on negative, rather then on positive supply

//Html page refresh
int refreshPage = 10; //default is 10sec.
//Beware that if you make it refresh too fast, the page could become inacessable.

//Display or hide the "Switch on all Pins" buttons at the bottom of page
int switchOnAllPinsButton = true; //true or false

//Button Array
//Just for a note, varables start from 0 to 9, as 0 is counted as well, thus 10 outputs.

// Select the pinout address
int outputAddress[04] = {5, 7, 6, 8 }; //Allocate 10 spaces and name the output pin address.


//PS pin addresses 10, 11, 12 and 13 on the Duemilanove are used for the ethernet shield, therefore cannot be used.
//PS pin addresses 10, 50, 51 and 52 and 53 on the Mega are used for the ethernet shield, therefore cannot be used.
//PS pin addresses 4, are used for the SD card, therefore cannot be used.
//PS. pin address 2 is used for interrupt-driven notification, therefore could not be used.

// Write the text description of the output channel
String buttonText[04] = {
  "WorkStation Lamp", "LEFT LED", "RIGHT LED", "Auto-Switched PSU"
};

// Set the output to retain the last status after power recycle.
int retainOutputStatus[04] = {1, 1, 1, 1}; //1-retain the last status. 0-will be off after power cut.

////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////
//VARIABLES DECLARATION
////////////////////////////////////////////////////////////////////////
int outp = 0;
boolean printLastCommandOnce = false;
boolean printButtonMenuOnce = false;
boolean initialPrint = true;
String allOn = "";
String allOff = "";
boolean reading = false;
boolean outputStatus[4]; //Create a boolean array for the maximum ammount.
String rev = "V7.00 (Date Modified : 25-12-15)";
String rev2lcd = "REV:7.00";
unsigned long timeConnectedAt;
boolean writeToEeprom = false;
//EthernetClient client;
/////////////////////////////////////////////////

// Temperature Related Reading
const int tempInPin = A0;
int tempInValue ; //temperature read
int tempOutDeg ;
int tempadjustment = 45;

// 2nd Temperature Related Reading
const int temp2InPin = A1;
int temp2InValue ; //temperature read
int temp2OutDeg ;
int temp2adjustment = 45;




///////////////////// Voltage Monitor Section Starts//////

int BatIn = A2;
float vout = 0.0;
float vin = 0.0;
float R1 = 100000.0; // resistance of R1 (100K) -see text!
float R2 = 10000.0; // resistance of R2 (10K) - see text!
int value = 0;

//////////////////////Voltage Monitor Section Ends////////
int sensorPin = A3;
float sample2 = 0; // for current
float val; // current callibration
float actualval; // read the actual current from ACS 712
float amps = 0.0;
float totamps = 0.0;
float avgamps = 0.0;
float amphr = 0.0;
float watt = 0.0;
float energy = 0.0;

///////////////////////////BIG FONT STARTS////////////////////////////////
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
///////////////////////////BIG FONT ENDS//////////////////////////////////


//RUN ONCE
////////////////////////////////////////////////////////////////////////
//Beginning of Program
void setup() {
pinMode(Button, INPUT);
digitalWrite(Button, HIGH); //Use internal pull up resistor for the button. It saved a resistor for the button.
// 
pinMode(Button2, INPUT);
digitalWrite(Button2, HIGH); //Use internal pull up resistor for the button. It saved a resistor for the button.

pinMode(Button3, INPUT);
digitalWrite(Button3, HIGH); //Use internal pull up resistor for the button. It saved a resistor for the button.

digitalWrite( outputAddress[00], HIGH);
digitalWrite( outputAddress[01], HIGH);
digitalWrite( outputAddress[02], HIGH);
  
 // activate LCD module
  lcd.begin (20, 4); // for 16 x 2 LCD module
  lcd.setBacklightPin(3, POSITIVE);
  lcd.setBacklight(HIGH);
  {
    Wire.begin();
    Serial.begin(9600);
  }
///////////////////////////BIG FONT STARTS//////////////////////////////////
  for (nb=0; nb<8; nb++ ) {                     // create 8 custom characters
    for (bc=0; bc<8; bc++) bb[bc]= pgm_read_byte( &custom[nb][bc] );
    lcd.createChar ( nb+1, bb );
  }
///////////////////////////BIG FONT ENDS//////////////////////////////////

 // SPLASH SCREEN START
   lcd.home (); // set cursor to 0,0
  lcd.setCursor (0, 0);
  lcd.write("....INITIALIZING...."); 
//  lcd.setCursor(0, 1);
//  lcd.write(".....Please Wait....");
  lcd.setCursor(0, 1);
  lcd.write("      Welcome  !!!  ");
//  lcd.setCursor(0, 2);
//  lcd.write("   Automation By:   ");
//  lcd.setCursor(0, 3);
//  lcd.write("      AL  AHSAN     ");
  writeBigString(" AHSAN", 0, 2);
  delay(2000);
  lcd.clear() ;
  //SPLASH SCREEN END 
  
  {
    
    Wire.begin();
    RTC.begin();
if( !RTC.isrunning() ) {
    Serial.println("RTC Not Configured");
    Serial.println("Starting Ethernet");  //configure Ethernet
    if(Ethernet.begin(mac) == 0) {
      Serial.println("Fatal Error: Unable to obtain DHCP address");
      for(;;)  //do nothing forever!
        ;      //TODO Change this to retry a few times
    }
    
    Serial.println("Ethernet Configured");
    Udp.begin(localPort);

    //get the NTP timestamp
    epoch = getNTP();
    
    //set the RTC
    RTC.adjust(epoch);
    
    //display what we did!
    Serial.println("RTC Configured:");
    
    //show Unix Epoch
    Serial.print("Unix Epoch: ");
    Serial.println(epoch);
    
    //show UTC
    Serial.print("UTC: ");
    showUTC(epoch);
  } else {
    Serial.println("RTC Already Configured");
  }
  
  }
  Serial.begin(9600);

  initEepromValues();
  readEepromValues();

  //Setting up the IP address. Comment out the one you dont need.
  //Ethernet.begin(mac); //for DHCP address. (Address will be printed to serial.)
  Ethernet.begin(mac, ip, gateway, subnet); //for manual setup. (Address is the one configured above.)


  server.begin();
  Serial.print("Server started at ");
  Serial.println(Ethernet.localIP());


  //Set pins as Outputs
  boolean currentState = false;
  for (int var = 0; var < outputQuantity; var++) {

    pinMode(outputAddress[var], OUTPUT);

    //Switch all outputs to either on or off on Startup
    if (outputInverted == true) {
      digitalWrite(outputAddress[var], HIGH);
      if (outputStatus[var] == 1) {
        currentState = true; //check outputStatus if off, switch output accordingly
      } else {
        currentState = false;
      }
      digitalWrite(outputAddress[var], currentState);

    }
    else {

      digitalWrite(outputAddress[var], LOW);
      if (outputStatus[var] == 1) {
        currentState = false; //check outputStatus if off, switch output accordingly
      } else {
        currentState = true;
      }
      digitalWrite(outputAddress[var], currentState);
    }

  }

  ///////////////////// Voltage Monitor Section Starts//////
  pinMode(BatIn, INPUT);

  //////////////////////Voltage Monitor Section Ends////////

 delay( 400 );
}


////////////////////////////////////////////////////////////////////////
//LOOP
////////////////////////////////////////////////////////////////////////
//Run once
void loop() {
  DateTime now = RTC.now();

///////////////////////////amps code starts/////////////
for (int i = 0; i < 150; i++)
  {
    // sample1+=analogRead(A2);  //read the voltage from the sensor
    sample2 += analogRead(A3); //read the current from sensor
    delay(2);
  }
  // sample1=sample1/150;
  sample2 = sample2 / 150;

  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  /// current calculation //////////////////////
  val = (5.0 * sample2) / 1024.0;
  actualval = val - 2.5; // offset voltage is 2.5v
  amps = actualval * 100; // 100mv/A from data sheet
  totamps = totamps + amps; // total amps
  //   avgamps=totamps/time; // average amps
  //   amphr=(avgamps*time)/3600;  // amphour
  //   watt =voltage*amps;    // power=voltage*current
  //   energy=(watt*time)/3600;   //energy in watt hour
  //   // energy=(watt*time)/(1000*3600);   // energy in kWh

  ////////////////////////////////////DISPLAY IN SERIAL MONITOR//////////////////////////////////////////////////////////

  //Serial.print("VOLTAGE : ");
  //Serial.print(voltage);
  //Serial.println("Volt");
  //Serial.print("CURRENT :");
  //Serial.print(outp);
  //Serial.println("Amps");
  //Serial.print("POWER :");
  //Serial.print(watt);
  //Serial.println("Watt");
  //Serial.print("ENERGY CONSUMED :");
  //Serial.print(energy);
  //Serial.println("Watt-Hour");
  //Serial.println(""); // print the next sets of parameter after a blank line
///////////////////////////amps code ends///////////////


  lcd.home (); // set cursor to 0,0
  lcd.setCursor (0, 0);
  
   if (now.hour() <10){  // Add a zero, if necessary, as above
lcd.print(0);
 }
////////////////////////////////24 hour to 12 conversion starts//////////////////////////////
if (now.hour() >= 13){
int thour = (now.hour() -12);
  if (thour <10){  // Add a zero, if necessary, as above
lcd.print(0);
  }
lcd.print(thour);
 }
else {
  lcd.print(now.hour()); // Display the current hour
}
////////////////////////////////24 hour to 12 conversion ends/////////////////////////////////
  lcd.setCursor (8, 0);
  
 switch (now.hour()){
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
 
  if (now.minute()<10){  // Add a zero, if necessary, as above
 lcd.print(0);
 }
 lcd.print(now.minute(), DEC); // Display the current minutes

  lcd.setCursor (5, 0);
  lcd.print(":");
  lcd.setCursor (6, 0);
  
  if (now.second()<10){  // Add a zero, if necessary, as above
 lcd.print(0);
 }
 lcd.print(now.second(), DEC); // Display the current seconds


  lcd.setCursor (11, 0);
  lcd.print(now.day(), DEC);

  lcd.setCursor (13, 0);
  lcd.print("- ");

  lcd.setCursor (14, 0);
 // lcd.print(now.month(), DEC);
 switch (now.month()){
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
 switch (now.year()){
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

  
  lcd.setCursor (0, 2);
  lcd.print("Bat V:");

if(vin>=27.75) {
    lcd.setCursor (6, 2);
  lcd.print("FULL ");
  }
else {  lcd.setCursor (6, 2);
  lcd.print(vin ,1);
  }
  
    lcd.setCursor (11, 2);
  lcd.print("Each:"); 
  
//  lcd.setCursor (5, 3);
//  lcd.print((vin/2));
  
if((vin/2)>=13.87) {
    lcd.setCursor (16, 2);
  lcd.print("FULL ");
  }
else {  lcd.setCursor (16, 2);
  lcd.print((vin/2));
  }

 lcd.setCursor (0, 3);
  lcd.print("AMPS:              ");

  lcd.setCursor (5, 3);
  lcd.print(amps); 

  lcd.setCursor (0, 1);
  lcd.print("Temp:");

  lcd.setCursor (5, 1);
  lcd.print(tempOutDeg);

  lcd.setCursor (8, 1);
  lcd.print("PSU Temp:");

  lcd.setCursor (17, 1);
  lcd.print(temp2OutDeg);

  lcd.setCursor (11, 3);
  lcd.print(rev2lcd);

if (temp2OutDeg > 100){ ////////////////////////////////// Change Temp Here !!!////////////////////////////
digitalWrite(outputAddress[03], LOW);
  lcd.setCursor (0, 3);
  lcd.print("PSU SAFTY SHUTDOWN");
   }
else {
  digitalWrite(outputAddress[03], HIGH);
  }
  
  //writeBigString( tempOutDeg , 17, 1);
  //  lcd.setBacklight(LOW);      // Backlight off
  //  delay(250);
  //  lcd.setBacklight(HIGH);     // Backlight on

  //Read Temperature Sensor
  tempInValue = analogRead(tempInPin);

  tempOutDeg = (5 * tempInValue * 100 / 1024) + tempadjustment;

  //Read 2nd Temperature Sensor
  temp2InValue = analogRead(temp2InPin);

  temp2OutDeg = (5 * temp2InValue * 100 / 1024) + temp2adjustment;

  ///////////////////// Voltage Monitor Section Starts//////
  // read the value at analog input
  value = analogRead(BatIn);
  vout = (value * 5.0) / 1024.0; // see text
  vin = vout / (R2 / (R1 + R2));
  if (vin < 0.09) {
    vin = 0.0; //statement to quash undesired reading !
  }
 


  // listen for incoming clients, and process requests.
  checkForClient();
/////////////////////////////////////LCD AUTO TURM OFF CODE STARTS////////////////////////////////
switch (now.hour()){
  case 1:
    lcd.setBacklight(LOW);
    break;
      case 2:
    lcd.setBacklight(LOW);
    break;
    case 3:
    lcd.setBacklight(LOW);
    break;
    case 4:
    lcd.setBacklight(LOW);
    break;
      case 5:
    lcd.setBacklight(LOW);
    break;
      case 6:
    lcd.setBacklight(LOW);
    break;
default:
    lcd.setBacklight(HIGH);
    break;
  }
  /////////////////////////////////////LCD AUTO TURM OFF CODE ENDS////////////////////////////////
  ///////////////1st relay///////////////////
{
if (digitalRead(Button)==HIGH) //if button is pressed
{
if ( outputAddress[00] == HIGH)
{//if the outputAddress[00] is off at this moment,
digitalWrite( outputAddress[00], LOW); //turn it on
outputStatus[0] = true; //new status of the outputAddress[00] is HIGH
}
else
{//if the outputAddress[00] is on at this moment,
digitalWrite( outputAddress[00], HIGH); //trun it off
outputStatus[0] = false; //new status of the outputAddress[00] is LOW 
} 
} 
//delay(100); //A very simple way to handle button bouncing, but need to adjust the delay time to optimize the result.
}
/////////2nd relay///////////////
{
if (digitalRead(Button2)==HIGH) //if button is pressed
{
if ( outputAddress[01] == LOW)
{//if the outputAddress[00] is off at this moment,
digitalWrite( outputAddress[01], HIGH); //turn it on
outputAddress[01] = HIGH; //new status of the outputAddress[00] is HIGH
}
else
{//if the outputAddress[00] is on at this moment,
digitalWrite( outputAddress[01], LOW); //trun it off
outputAddress[01] = LOW; //new status of the outputAddress[00] is LOW 
} 
} 
//delay(100); //A very simple way to handle button bouncing, but need to adjust the delay time to optimize the result.
}
////////3 relay///////////////////
{
if (digitalRead(Button3)==HIGH) //if button is pressed
{
if ( outputAddress[02] == LOW)
{//if the outputAddress[00] is off at this moment,
digitalWrite( outputAddress[02], HIGH); //turn it on
outputAddress[02] = HIGH; //new status of the outputAddress[00] is HIGH
}
else
{//if the outputAddress[00] is on at this moment,
digitalWrite( outputAddress[02], LOW); //trun it off
outputAddress[02] = LOW; //new status of the outputAddress[00] is LOW 
} 
} 
//delay(100); //A very simple way to handle button bouncing, but need to adjust the delay time to optimize the result.
}
}

//////////////////////////////////////auto ntp code starts//////////////////////////////
void showUTC(unsigned long epoch) {
  Serial.print((epoch  % 86400L) / 3600); // print the hour (86400 equals secs per day)
  Serial.print(':');  
  if ( ((epoch % 3600) / 60) < 10 ) {
    // In the first 10 minutes of each hour, we'll want a leading '0'
    Serial.print('0');
  }
  Serial.print((epoch  % 3600) / 60); // print the minute (3600 equals secs per minute)
  Serial.print(':'); 
  if ( (epoch % 60) < 10 ) {
    // In the first 10 seconds of each minute, we'll want a leading '0'
    Serial.print('0');
  }
  Serial.println(epoch %60); // print the second
}

unsigned long getNTP() {
  sendNTPpacket(timeServer); // send an NTP packet to a time server

  // wait to see if a reply is available
  delay(1000);  
  if ( Udp.parsePacket() ) {  
    // We've received a packet, read the data from it
    Udp.read(packetBuffer,NTP_PACKET_SIZE);  // read the packet into the buffer

    //the timestamp starts at byte 40 of the received packet and is four bytes,
    // or two words, long. First, esxtract the two words:

    unsigned long highWord = word(packetBuffer[40], packetBuffer[41]);
    unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);  
    // combine the four bytes (two words) into a long integer
    // this is NTP time (seconds since Jan 1 1900):
    unsigned long secsSince1900 = highWord << 16 | lowWord;  
    Serial.print("Seconds since Jan 1 1900 = " );
    Serial.println(secsSince1900);               

    // now convert NTP time into everyday time:
    Serial.print("Unix time = ");
    // Unix time starts on Jan 1 1970. In seconds, that's 2208988800:
    const unsigned long seventyYears = 2208988800UL;     
    // subtract seventy years:
    unsigned long epoch = secsSince1900 - seventyYears;  
    
    return epoch;
  }
}

// send an NTP request to the time server at the given address 
void sendNTPpacket(IPAddress& address)
{
  // set all bytes in the buffer to 0
  memset(packetBuffer, 0, NTP_PACKET_SIZE); 
  // Initialize values needed to form NTP request
  // (see URL above for details on the packets)
  packetBuffer[0] = 0b11100011;   // LI, Version, Mode
  packetBuffer[1] = 0;     // Stratum, or type of clock
  packetBuffer[2] = 6;     // Polling Interval
  packetBuffer[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  packetBuffer[12]  = 49; 
  packetBuffer[13]  = 0x4E;
  packetBuffer[14]  = 49;
  packetBuffer[15]  = 52;

  // all NTP fields have been given values, now
  // you can send a packet requesting a timestamp:         
  Udp.beginPacket(address, 123); //NTP requests are to port 123
  Udp.write(packetBuffer,NTP_PACKET_SIZE);
  Udp.endPacket(); 
}
//////////////////////////////////////auto ntp code ends////////////////////////////////


/////////////////////////////////BIG FONT STARTS AGAIN/////////////////////////////////
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
/////////////////////////////////BIG FONT ENDS AGAIN/////////////////////////////////

//checkForClient Function
////////////////////////////////////////////////////////////////////////
//
void checkForClient() {

  EthernetClient client = server.available();

  if (client) {

    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    boolean sentHeader = false;

    while (client.connected()) {
      if (client.available()) {

        //if header was not set send it

        //read user input
        char c = client.read();

        if (c == '*') {

          printHtmlHeader(client); //call for html header and css
          printLoginTitle(client);
          printHtmlFooter(client);
          //sentHeader = true;
          break;
        }

        if (!sentHeader) {

          printHtmlHeader(client); //call for html header and css
          printHtmlButtonTitle(client); //print the button title

          //This is for the arduino to construct the page on the fly.
          sentHeader = true;
        }

        //read user input
        //    char c = client.read();

        //if there was reading but is blank there was no reading
        if (reading && c == ' ') {
          reading = false;
        }

        //if there is a ? there was user input
        if (c == '?') {
          reading = true; //found the ?, begin reading the info
        }

        // if there was user input switch the relevant output
        if (reading) {

          //if user input is H set output to 1
          if (c == 'H') {
            outp = 1;
          }

          //if user input is L set output to 0
          if (c == 'L') {
            outp = 0;
          }

          Serial.print(c);   //print the value of c to serial communication
          //Serial.print(outp);
          //Serial.print('\n');

          switch (c) {

            case '0':
              //add code here to trigger on 0
              triggerPin(outputAddress[0], client, outp);
              break;
            case '1':
              //add code here to trigger on 1
              triggerPin(outputAddress[1], client, outp);
              break;
            case '2':
              //add code here to trigger on 2
              triggerPin(outputAddress[2], client, outp);
              break;
            case '3':
              //add code here to trigger on 3
              triggerPin(outputAddress[3], client, outp);
              break;

          } //end of switch case

        }//end of switch switch the relevant output

        //if user input was blank
        if (c == '\n' && currentLineIsBlank) {
          printLastCommandOnce = true;
          printButtonMenuOnce = true;
          triggerPin(777, client, outp); //Call to read input and print menu. 777 is used not to update any outputs
          break;
        }




      }
    }

    printHtmlFooter(client); //Prints the html footer

  }
  else
  { //if there is no client

    //And time of last page was served is more then a minute.
    if (millis() > (timeConnectedAt + 30000)) {

      if (writeToEeprom == true) {
        writeEepromValues();  //write to EEprom the current output statuses
        Serial.println("No Clients for more then a minute - Writing statuses to Eeprom.");
        writeToEeprom = true;
      }

    }
  }


}


////////////////////////////////////////////////////////////////////////
//triggerPin Function
////////////////////////////////////////////////////////////////////////
//
void triggerPin(int pin, EthernetClient client, int outp) {
  //Switching on or off outputs, reads the outputs and prints the buttons

  //Setting Outputs
  if (pin != 777) {

    if (outp == 1) {
      if (outputInverted == false) {
        digitalWrite(pin, HIGH);
      }
      else {
        digitalWrite(pin, LOW);
      }
    }
    if (outp == 0) {
      if (outputInverted == false) {
        digitalWrite(pin, LOW);
      }
      else {
        digitalWrite(pin, HIGH);
      }
    }


  }
  //Refresh the reading of outputs
  readOutputStatuses();


  //Prints the buttons
  if (printButtonMenuOnce == true) {
    printHtmlButtons(client);
    printButtonMenuOnce = false;
  }

}

////////////////////////////////////////////////////////////////////////
//printHtmlButtons Function
////////////////////////////////////////////////////////////////////////
//print the html buttons to switch on/off channels
void printHtmlButtons(EthernetClient client) {

  //Start to create the html table
  client.println("");
  //client.println("<p>");
  client.println("<FORM>");
  client.println("<table border=\"0\" align=\"center\">");


  //Printing the Temperature
  client.print("<tr>\n");

  client.print("<td><h4>");
  client.print(" Room Temp");
  client.print("</h4></td>\n");

  client.print("<td>");
  client.print("<h3>");
  client.print(tempOutDeg);

  client.print(" &deg;C</h3></td>\n");


  client.print("<td></td>");
  client.print("</tr>");

  //Printing the Temperature
  client.print("<tr>\n");

  client.print("<td><h4>");
  client.print("PSU Temp");
  client.print("</h4></td>\n");

  client.print("<td>");
  client.print("<h3>");
  client.print(temp2OutDeg);

  client.print(" &deg;C</h3></td>\n");


  client.print("<td></td>");
  client.print("</tr>");

  //Printing the Battery voltage
  client.print("<tr>\n");

  client.print("<td><h4>");
  client.print("UPS Battery ");
  client.print("</h4></td>\n");

  client.print("<td>");
  client.print("<h3>");
  client.print(vin);
  client.print("V");//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  client.print("(Each:");
//  client.print(vin / 2);
//  client.print("V)");
  
if((vin/2)>=13.87) {
    client.print("(Battery ");
    client.print("FULL or Above 13.88 Volts)");
    
  }
else { client.print("(Each:");  
  client.print(vin / 2);
  client.print("V)");
  }


  //Printing the Amps
  client.print("<tr>\n");

  client.print("<td><h4>");
  client.print("Total Amps In Use ");
  client.print("</h4></td>\n");

  client.print("<td>");
  client.print("<h3>");
  client.print(amps);
  client.print("Amps");
  
  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    

  

  //Start printing button by button
  for (int var = 0; var < outputQuantity; var++)  {

    //set command for all on/off
    allOn += "H";
    allOn += outputAddress[var];
    allOff += "L";
    allOff += outputAddress[var];


    //Print begining of row
    client.print("<tr>\n");

    //Prints the button Text
    client.print("<td><h4>");
    client.print(buttonText[var]);
    client.print("</h4></td>\n");

    //Prints the ON Buttons
    client.print("<td>");
    //client.print(buttonText[var]);
    client.print("<INPUT TYPE=\"button\" VALUE=\"ON ");
    //client.print(buttonText[var]);
    client.print("\" onClick=\"parent.location='/?H");
    client.print(var);
    client.print("'\"></td>\n");

    //Prints the OFF Buttons
    client.print(" <td><INPUT TYPE=\"button\" VALUE=\"OFF");
    //client.print(var);
    client.print("\" onClick=\"parent.location='/?L");
    client.print(var);
    client.print("'\"></td>\n");


    //Print first part of the Circles or the LEDs

    //Invert the LED display if output is inverted.

    if (outputStatus[var] == true ) {                                                           //If Output is ON
      if (outputInverted == false) {                                                            //and if output is not inverted
        client.print(" <td><div class='green-circle'><div class='glare'></div></div></td>\n"); //Print html for ON LED
      }
      else {                                                                                   //else output is inverted then
        client.print(" <td><div class='black-circle'><div class='glare'></div></div></td>\n"); //Print html for OFF LED
      }
    }
    else                                                                                      //If Output is Off
    {
      if (outputInverted == false) {                                                          //and if output is not inverted
        client.print(" <td><div class='black-circle'><div class='glare'></div></div></td>\n"); //Print html for OFF LED
      }
      else {                                                                                  //else output is inverted then
        client.print(" <td><div class='green-circle'><div class='glare'></div></div></td>\n"); //Print html for ON LED
      }
    }




    //Print end of row
    client.print("</tr>\n");
  }

  //Display or hide the Print all on Pins Button
  if (switchOnAllPinsButton == false ) {

    //Prints the ON All Pins Button
    client.print("<tr>\n<td><INPUT TYPE=\"button\" VALUE=\"Switch ON All Pins");
    client.print("\" onClick=\"parent.location='/?");
    client.print(allOn);
    client.print("'\"></td>\n");

    //Prints the OFF All Pins Button
    client.print("<td><INPUT TYPE=\"button\" VALUE=\"Switch OFF All Pins");
    client.print("\" onClick=\"parent.location='/?");
    client.print(allOff);
    client.print("'\"></td>\n<td></td>\n<td></td>\n</tr>\n");
  }

  //Closing the table and form
  client.println("</table>");
  client.println("</FORM>");
  //client.println("</p>");

}

////////////////////////////////////////////////////////////////////////
//readOutputStatuses Function
////////////////////////////////////////////////////////////////////////
//Reading the Output Statuses
void readOutputStatuses() {
  for (int var = 0; var < outputQuantity; var++)  {
    outputStatus[var] = digitalRead(outputAddress[var]);
    //Serial.print(outputStatus[var]);
  }

}

////////////////////////////////////////////////////////////////////////
//readEepromValues Function
////////////////////////////////////////////////////////////////////////
//Read EEprom values and save to outputStatus
void readEepromValues() {
  for (int adr = 0; adr < outputQuantity; adr++)  {
    outputStatus[adr] = EEPROM.read(adr);
  }
}

////////////////////////////////////////////////////////////////////////
//writeEepromValues Function
////////////////////////////////////////////////////////////////////////
//Write EEprom values
void writeEepromValues() {
  for (int adr = 0; adr < outputQuantity; adr++)  {
    EEPROM.write(adr, outputStatus[adr]);
  }

}

////////////////////////////////////////////////////////////////////////
//initEepromValues Function
////////////////////////////////////////////////////////////////////////
//Initialiaze EEprom values
//if eeprom values are not the correct format ie not euqual to 0 or 1 (thus greater then 1) initialize by putting 0
void initEepromValues() {
  for (int adr = 0; adr < outputQuantity; adr++) {
    if (EEPROM.read(adr) > 1) {
      EEPROM.write(adr, 0);
    }

  }

}


////////////////////////////////////////////////////////////////////////
//htmlHeader Function
////////////////////////////////////////////////////////////////////////
//Prints html header
void printHtmlHeader(EthernetClient client) {
  Serial.print("Serving html Headers at ms -");
  timeConnectedAt = millis(); //Record the time when last page was served.
  Serial.print(timeConnectedAt); // Print time for debbugging purposes
  writeToEeprom = true; // page loaded so set to action the write to eeprom

  // send a standard http response header
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("Connnection: close");
  client.println();
  client.println("<!DOCTYPE HTML>");
  client.println("<head>");

  // add page title
  client.println("<title>Ethernet Switching</title>");
  client.println("<meta name=\"description\" content=\"LAB SWITCHBOARD\"/>");

  // add a meta refresh tag, so the browser pulls again every x seconds:
  client.print("<meta http-equiv=\"refresh\" content=\"");
  client.print(refreshPage);
  client.println("; url=/\">");

  // add other browser configuration
  client.println("<meta name=\"apple-mobile-web-app-capable\" content=\"yes\">");
  client.println("<meta name=\"apple-mobile-web-app-status-bar-style\" content=\"default\">");
  client.println("<meta name=\"viewport\" content=\"width=device-width, user-scalable=no\">");

  //inserting the styles data, usually found in CSS files.
  client.println("<style type=\"text/css\">");
  client.println("");

  //This will set how the page will look graphically
  client.println("html { height:100%; }");

  client.println("  body {");
  client.println("    height: 100%;");
  client.println("    margin: 0;");
  client.println("    font-family: helvetica, sans-serif;");
  client.println("    -webkit-text-size-adjust: none;");
  client.println("   }");
  client.println("");
  client.println("body {");
  client.println("    -webkit-background-size: 100% 21px;");
  client.println("    background-color: #FDEDD0;");
  client.println("    background-image:");
  client.println("    -webkit-gradient(linear, left top, right top,");
  client.println("    color-stop(.75, transparent),");
  client.println("    color-stop(.75, rgba(255,255,255,.1)) );");
  client.println("    -webkit-background-size: 7px;");
  client.println("   }");
  client.println("");
  client.println(".view {");
  client.println("    min-height: 100%;");
  client.println("    overflow: auto;");
  client.println("   }");
  client.println("");
  client.println(".header-wrapper {");
  client.println("    height: 44px;");
  client.println("    font-weight: bold;");
  client.println("    text-shadow: rgba(0,0,0,0.7) 0 -1px 0;");
  client.println("    border-top: solid 1px rgba(255,255,255,0.6);");
  client.println("    border-bottom: solid 1px rgba(0,0,0,0.6);");
  client.println("    color: #fff;");
  client.println("    background-color: #FF634D;");
  client.println("    background-image:");
  client.println("    -webkit-gradient(linear, left top, left bottom,");
  client.println("    from(rgba(255,255,255,.4)),");
  client.println("    to(rgba(255,255,255,.05)) ),");
  client.println("    -webkit-gradient(linear, left top, left bottom,");
  client.println("    from(transparent),");
  client.println("    to(rgba(0,0,64,.1)) );");
  client.println("    background-repeat: no-repeat;");
  client.println("    background-position: top left, bottom left;");
  client.println("    -webkit-background-size: 100% 21px, 100% 22px;");
  client.println("    -webkit-box-sizing: border-box;");
  client.println("   }");
  client.println("");
  client.println(".header-wrapper h1 {");
  client.println("    text-align: center;");
  client.println("    font-size: 20px;");
  client.println("    line-height: 44px;");
  client.println("    margin: 0;");
  client.println("   }");
  client.println("");
  client.println(".group-wrapper {");
  client.println("    margin: 9px;");
  client.println("    }");
  client.println("");
  client.println(".group-wrapper h2 {");
  client.println("    text-align: center;");
  client.println("    color: #FD795B;");
  client.println("    font-size: 17px;");
  client.println("    line-height: 0.8;");
  client.println("    font-weight: bold;");
  client.println("    text-shadow: #fff 0 1px 0;");
  client.println("    margin: 20px 10px 12px;");
  client.println("   }");
  client.println("");
  client.println(".group-wrapper h3 {");
  client.println("    color: #FD795B;");
  client.println("    font-size: 12px;");
  client.println("    line-height: 1;");
  client.println("    font-weight: bold;");
  client.println("    text-shadow: #fff 0 1px 0;");
  client.println("    margin: 20px 10px 12px;");
  client.println("   }");
  client.println("");
  client.println(".group-wrapper h4 {");  //Text for description
  client.println("    color: #FD212121;");
  client.println("    font-size: 14px;");
  client.println("    line-height: 1;");
  client.println("    font-weight: bold;");
  client.println("    text-shadow: #aaa 1px 1px 3px;");
  client.println("    margin: 5px 5px 5px;");
  client.println("   }");
  client.println("");
  client.println(".group-wrapper table {");
  client.println("    background-color: #BCF1ED;");
  client.println("    -webkit-border-radius: 10px;");
  client.println("    -moz-border-radius: 10px;");
  client.println("    -khtml-border-radius: 10px;");
  client.println("    border-radius: 10px;");
  client.println("    font-size: 17px;");
  client.println("    line-height: 20px;");
  client.println("    margin: 9px 0 20px;");
  client.println("    border: solid 1px #BCF1ED;");
  client.println("    padding: 11px 3px 12px 3px;");
  client.println("    margin-left:auto;");
  client.println("    margin-right:auto;");

  client.println("    -moz-transform :scale(1);"); //Code for Mozilla Firefox
  client.println("    -moz-transform-origin: 0 0;");



  client.println("   }");
  client.println("");


  //how the green (ON) LED will look
  client.println(".green-circle {");
  client.println("    display: block;");
  client.println("    height: 23px;");
  client.println("    width: 23px;");
  client.println("    background-color: #0f0;");
  //client.println("    background-color: rgba(60, 132, 198, 0.8);");
  client.println("    -moz-border-radius: 11px;");
  client.println("    -webkit-border-radius: 11px;");
  client.println("    -khtml-border-radius: 11px;");
  client.println("    border-radius: 11px;");
  client.println("    margin-left: 1px;");

  client.println("    background-image: -webkit-gradient(linear, 0% 0%, 0% 90%, from(rgba(46, 184, 0, 0.8)), to(rgba(148, 255, 112, .9)));@");
  client.println("    border: 2px solid #ccc;");
  client.println("    -webkit-box-shadow: rgba(11, 140, 27, 0.5) 0px 10px 16px;");
  client.println("    -moz-box-shadow: rgba(11, 140, 27, 0.5) 0px 10px 16px; /* FF 3.5+ */");
  client.println("    box-shadow: rgba(11, 140, 27, 0.5) 0px 10px 16px; /* FF 3.5+ */");

  client.println("    }");
  client.println("");

  //how the black (off)LED will look
  client.println(".black-circle {");
  client.println("    display: block;");
  client.println("    height: 23px;");
  client.println("    width: 23px;");
  client.println("    background-color: #040;");
  client.println("    -moz-border-radius: 11px;");
  client.println("    -webkit-border-radius: 11px;");
  client.println("    -khtml-border-radius: 11px;");
  client.println("    border-radius: 11px;");
  client.println("    margin-left: 1px;");
  client.println("    -webkit-box-shadow: rgba(11, 140, 27, 0.5) 0px 10px 16px;");
  client.println("    -moz-box-shadow: rgba(11, 140, 27, 0.5) 0px 10px 16px; /* FF 3.5+ */");
  client.println("    box-shadow: rgba(11, 140, 27, 0.5) 0px 10px 16px; /* FF 3.5+ */");
  client.println("    }");
  client.println("");

  //this will add the glare to both of the LEDs
  client.println("   .glare {");
  client.println("      position: relative;");
  client.println("      top: 1;");
  client.println("      left: 5px;");
  client.println("      -webkit-border-radius: 10px;");
  client.println("      -moz-border-radius: 10px;");
  client.println("      -khtml-border-radius: 10px;");
  client.println("      border-radius: 10px;");
  client.println("      height: 1px;");
  client.println("      width: 13px;");
  client.println("      padding: 5px 0;");
  client.println("      background-color: rgba(200, 200, 200, 0.25);");
  client.println("      background-image: -webkit-gradient(linear, 0% 0%, 0% 95%, from(rgba(255, 255, 255, 0.7)), to(rgba(255, 255, 255, 0)));");
  client.println("    }");
  client.println("");


  //and finally this is the end of the style data and header
  client.println("</style>");
  client.println("</head>");

  //now printing the page itself
  client.println("<body>");
  client.println("<div class=\"view\">");
  client.println("    <div class=\"header-wrapper\">");
  client.println("      <h1>LAB Lighing Board</h1>");
  client.println("    </div>");

  //////

} //end of htmlHeader

////////////////////////////////////////////////////////////////////////
//htmlFooter Function
////////////////////////////////////////////////////////////////////////
//Prints html footer
void printHtmlFooter(EthernetClient client) {
  //Set Variables Before Exiting
  printLastCommandOnce = false;
  printButtonMenuOnce = false;
  allOn = "";
  allOff = "";

  //printing last part of the html
  client.println("\n<h3 align=\"center\">&copy; Automation By AL AHSAN - ");
  client.println(rev);
  client.println("</h3></div>\n</div>\n</body>\n</html>");

  delay(1); // give the web browser time to receive the data

  client.stop(); // close the connection:

  Serial.println(" - Done, Closing Connection.");

  delay (2); //delay so that it will give time for client buffer to clear and does not repeat multiple pages.

} //end of htmlFooter


////////////////////////////////////////////////////////////////////////
//printHtmlButtonTitle Function
////////////////////////////////////////////////////////////////////////
//Prints html button title
void printHtmlButtonTitle(EthernetClient client) {
  client.println("<div  class=\"group-wrapper\">");
  client.println("    <h2>Please Switch Your Selection</h2>");
  client.println();
}


////////////////////////////////////////////////////////////////////////
//printLoginTitle Function
////////////////////////////////////////////////////////////////////////
//Prints html button title
void printLoginTitle(EthernetClient client) {
//    client.println("<div  class=\"group-wrapper\">");
  client.println("    <h2>Please enter the user data to login.</h2>");
  client.println();
}
void sendData() {
  long milisec = millis(); // calculate time in milisec
  long time = milisec / 1000; // convert time to sec

  
}
