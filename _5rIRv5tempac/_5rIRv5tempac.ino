// Sketch by Ahsan Muhammad to Control four Manual Relays and one thermally active relay all By SERIAL COMMANDS AND 3 Different IR REMOTEs 
// V.1.0 Date : 1-2-14
#include <IRremote.h>
#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2); // initialize the library with the numbers of the interface pins
int IR_LED = 6;            //ir receiver
int R1 = 10;               // R1 pin location
int R2 = 9;                // R2 pin location
int R3 = 8;                // R3 pin location
int R4 = 7;                // R4 pin location
int R5 = 15;               // R5 pin location (ANALOG PIN # 02)
int Pled = 13;             // Power LED NOT TO BE CHANGED OR USED BY ANY OTHER ITEM
const int inPin = 0;       // analog pin
float sinVal; 
int ledVal;
int on = 0;
IRrecv irrecv(IR_LED);
decode_results results;
int R1S = 0;               // S stands for State of R1
int R2S = 0;
int R3S = 0;
int R4S = 0;              // to turn off the R4 on bootup
int R5S = 0;               // S stands for State of R1


void setup()
{
  pinMode(R1, OUTPUT);
  pinMode(R2, OUTPUT);
  pinMode(R3, OUTPUT);
  pinMode(R4, OUTPUT);
  pinMode(R5, OUTPUT);
  pinMode(Pled, OUTPUT); 
  pinMode(IR_LED, INPUT) ;
  // digitalWrite (R4 , HIGH); // to turn off the R4 on bootup
  digitalWrite (Pled , HIGH);
  
  Serial.begin(9600);      
  irrecv.enableIRIn(); // Start the receiver
 
  //DEF BOOT_UP RELAY SETTINGS SATART...
  digitalWrite(R1, LOW);
  digitalWrite(R2, LOW);       
  digitalWrite(R3, LOW);
  digitalWrite(R4, LOW); 
 // digitalWrite(R5, LOW);  // THERMAL ACTIVE RELAY 
  // DEF RELAY SETTINGS END...
  // LCD CONFIG STARTS FROM HERE 
  lcd.begin(16, 2);
  // SPLASH SCREEN START
  lcd.setCursor(0,0);
  lcd.write("INITIALIZING...."); 
  lcd.setCursor(0,1);
  lcd.write("PID:5rIRv5tempac");
  delay(2500);
  lcd.clear() ;
  lcd.setCursor(0,0);
  lcd.write("  Welcome Sir! ");
  delay(2000);
  lcd.clear() ;
  lcd.setCursor(0,0);
  lcd.write(" Automation By: ");
  lcd.setCursor(0,1);
  lcd.write("    AL AHSAN   ");
  delay(1500);
  lcd.clear() ;
  //SPLASH SCREEN END
  //STATIC DISPLAY MENU STARTS
  lcd.setCursor(0,0);
  lcd.write("R1");
  lcd.setCursor(0,1);
  lcd.write("DF");
  lcd.setCursor(2,0);
  lcd.write("R2");
  lcd.setCursor(2,1);
  lcd.write("DF");
  lcd.setCursor(4,0);
  lcd.write("R3");
  lcd.setCursor(4,1);
  lcd.write("DF");
  lcd.setCursor(6,0);
  lcd.write("R4");
  lcd.setCursor(6,1);
  lcd.write("DF");
  lcd.setCursor(8,0);
  lcd.write("R5");
  lcd.setCursor(8,1);
  lcd.write("DF");
  lcd.setCursor(11,0);
  lcd.write("Temp:");
  lcd.setCursor(10,0);
  lcd.write("|");
  lcd.setCursor(10,1);
  lcd.write("|");
}

unsigned long last = millis();

void loop()
{
  // IR REMOTE COMMANDS STARTS.....
  if (irrecv.decode(&results)) {
    if (results.value == 0xFF30CF || results.value == 0xB8E345C3 || results.value == 0x800F8401) { // 1 button for R1 B8E345C3 for n900 controll
      if (millis() - last > 250) {
        on = !on; // IR received, toggle the relay
        digitalWrite(R1, on ? HIGH : LOW);
        Serial.println("R1 toggled (Remote Command)");  
      }
      last = millis();
    }    
    irrecv.resume(); // Receive the next value


      if (results.value == 0xFF18E7 || results.value == 0x18E78877 || results.value == 0x800F0402) { // 2 button for R2
      if (millis() - last > 200) {
        on = !on; // IR received, toggle the relay
        digitalWrite(R2, on ? HIGH : LOW);
        Serial.println("R2 toggled (Remote Command)"); 
        last = millis();
      }    
      irrecv.resume(); // Receive the next value
    }
    if (results.value == 0xFF7A85 || results.value == 0x18E748B7 || results.value == 0x800F8403 ) { // 3 button for r3
      if (millis() - last > 200) {
        on = !on; // IR received, toggle the relay
        digitalWrite(R3, on ? HIGH : LOW);
        Serial.println("R3 toggled (Remote Command)"); 
      }
      last = millis();
      irrecv.resume(); // Receive the next value
    }  
    if (results.value == 0x800F0404 ) { // 4 button for r4
      if (millis() - last > 200) {
        on = !on; // IR received, toggle the relay
        digitalWrite(R4, on ? HIGH : LOW);
        Serial.println("R3 toggled (Remote Command)"); 
      }
      last = millis();
      irrecv.resume(); // Receive the next value
    }   
    if (results.value == 0xFFB04F || results.value == 0x18E758A7 || results.value == 0x800F041D) { // FL- button to deactivate every relay
      if (millis() - last > 200) {
        digitalWrite(R1, LOW);
        lcd.setCursor(0,1); //R1 LCD POSITION
        lcd.write("OF"); 
        digitalWrite(R2, LOW);
        lcd.setCursor(2,1); //R2 LCD POSITION
        lcd.write("OF");         
        digitalWrite(R3, LOW);
        lcd.setCursor(4,1); //R3 LCD POSITION
        lcd.write("OF"); 
        digitalWrite(R4, LOW);  // inverted coz on pnp instead of pnp
        lcd.setCursor(6,1); //R4 LCD POSITION
        lcd.write("OF"); 
        Serial.println("All Relays De-Energized (Remote Command)");  
      }
      last = millis();
    }    
    irrecv.resume(); // Receive the next value
    if (results.value == 0xFF9867 || results.value == 0x1D742F28 || results.value == 0x800F841C) { // FL+ button to activate every relay
      if (millis() - last > 200) {
        on = !on; // IR received, toggle the relay
        digitalWrite(R1, HIGH);
        lcd.setCursor(0,1); //R1 LCD POSITION
        lcd.write("ON"); 
        digitalWrite(R2, HIGH);
        lcd.setCursor(2,1); //R2 LCD POSITION
        lcd.write("ON");         
        digitalWrite(R3, HIGH);
        lcd.setCursor(4,1); //R3 LCD POSITION
        lcd.write("ON"); 
        digitalWrite(R4, HIGH);  // inverted coz on pnp instead of pnp
        lcd.setCursor(6,1); //R4 LCD POSITION
        lcd.write("ON"); 
        Serial.println("All Relays Energized (Remote Command)");  
      }
      last = millis();
    }
    irrecv.resume(); // Receive the next value
  }
  // IR REMOTE COMMANDS STARTS.....

  // THERMAL ACTIVE RELAY CODE STARTS...

  int value = analogRead(inPin); // taken from arduino cook book 
  Serial.print(value); 
  Serial.print(" > ");
  float millivolts = (value / 1024.0) * 5000; // sensor output is 10mV per degree Kelvin, 0 Celsius is 273.15
  float celsius = (millivolts / 10) - 273.15 ;
  Serial.print(celsius);
  Serial.println(" degrees Celsius, ");
  lcd.setCursor(11,1); // TEMP POSITION ON LCD
  lcd.print(celsius);
  delay(200); // wait for 200 mili-second
  if(celsius >= 50) // ################################# || set temp here || ##########################
  {
    
    digitalWrite(R5, HIGH);
    lcd.setCursor(8,1);
        lcd.write("ON");      
    lcd.setCursor(10,1);
        lcd.write("!");
        R5S= 1; 		
}
  else
  {
    digitalWrite(R5, LOW);
    lcd.setCursor(8,1);
        lcd.write("OF");
       lcd.setCursor(10,1);
        lcd.write("|");     
        R5S= 0;
        
  }
// THERMAL ACTIVE RELAY CODE ENDS...
}
