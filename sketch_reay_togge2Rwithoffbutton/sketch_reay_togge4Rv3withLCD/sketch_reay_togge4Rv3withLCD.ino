// Sketch by Ahsan Muhammad to Controll three Manual Relays and one thermally active relay By SERIAL COMMANDS AND A 3 Differnet IR REMOTEs 
// V.3.0 Date : 21-11-13
#include <IRremote.h>
#include <LiquidCrystal.h>
// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
int IR_LED = 6;
int R1 = 10;
int R2 = 9;
int R3 = 8;
int R4 = 7;
int Pled = 13;
int on = 0;
const int inPin = 0; // analog pin for Temprature Censor
float sinVal;
int ledVal;
IRrecv irrecv(IR_LED);
decode_results results;

void setup()
{
  lcd.begin(16, 2);
  lcd.setCursor(0,0);
  lcd.write("  Welcome Sir! ");
  delay(2000);
  pinMode(R1, OUTPUT);
  pinMode(R2, OUTPUT);
  pinMode(R3, OUTPUT);
  pinMode(R4, OUTPUT);
  pinMode(Pled, OUTPUT); 
  pinMode(IR_LED, INPUT) ;
  digitalWrite (R4 , HIGH); // to turn off the R4 on bootup
  digitalWrite (Pled , HIGH);
  Serial.begin(9600);      
  irrecv.enableIRIn(); // Start the receiver
} 
unsigned long last = millis();

void loop() 
{
  lcd.clear() ;
  lcd.setCursor(0,0);
  lcd.write("Temp:");
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
        on = !on; // IR received, toggle the relay
        digitalWrite(R1, LOW);
        digitalWrite(R2, LOW);
        digitalWrite(R3, LOW);
        digitalWrite(R4, HIGH); // inverted coz on pnp instead of pnp
        Serial.println("All Relays De-Energized (Remote Command)");  
      }
      last = millis();
    }    
    irrecv.resume(); // Receive the next value
    if (results.value == 0xFF9867 || results.value == 0x1D742F28 || results.value == 0x800F841C) { // FL+ button to activate every relay
      if (millis() - last > 200) {
        on = !on; // IR received, toggle the relay
        digitalWrite(R1, HIGH);
        digitalWrite(R2, HIGH);
        digitalWrite(R3, HIGH);
        digitalWrite(R4, LOW);  // inverted coz on pnp instead of pnp
        Serial.println("All Relays Energized (Remote Command)");  
      }
      last = millis();
    }
    irrecv.resume(); // Receive the next value
  }

  int value = analogRead(inPin); // taken from arduino cook book 
  Serial.print(value); 
  Serial.print(" > ");
  float millivolts = (value / 1024.0) * 5000;
  // sensor output is 10mV per degree Kelvin, 0 Celsius is 273.15
  float celsius = (millivolts / 10) - 273.15 ;
  Serial.print(celsius);
  Serial.println(" degrees Celsius, ");
  lcd.setCursor(5,0);
  lcd.print(celsius);
  delay(200); // wait for one second
  if(celsius >= 50) // ################################# || set temp here || ##########################
  {
    digitalWrite(R4, LOW);
  }
  else
  {
    digitalWrite(R4, HIGH);
    //delay(500); // wait for one second
  }
  {
    char ch = Serial.read();
    if (ch=='1'){
      digitalWrite (R1 ,HIGH);
      Serial.println("R1 Energized (Console Command)");
    lcd.setCursor(0,1);
      lcd.print("R1 Toggled(SER)");  
    }
    {
      if (ch=='2'){
        digitalWrite (R1 ,LOW);
        Serial.println("R1 De-Energized (Console Command)");    
      }
    }
    {
      if (ch=='3'){
        digitalWrite (R2 ,HIGH);
        digitalWrite(12,HIGH);
        Serial.println("R2 Energized (Console Command)");    
      }
    }
    {
      if (ch=='4'){
        digitalWrite (R2 ,LOW);
        digitalWrite(12,LOW);
        Serial.println("R2 De-Energized (Console Command)");    
      }
    }
    {
      if (ch=='5'){
        digitalWrite (R3 ,HIGH);

        Serial.println("R3 Energized (Console Command)");    
      }
    } 
    {
      if (ch=='6'){
        digitalWrite (R3 ,LOW);
        Serial.println("R3 De-Energized (Console Command)");    
      }
    }
    {
      if (ch=='7'){
        digitalWrite (R4 ,LOW); // INVERTED COZ OF THE PNP INSTEAD OF NPN TRANSISTOR
        Serial.println("R4 Energized (Console Command)");    
      } 
    } 
    {
      if (ch=='8'){
        digitalWrite (R4 ,HIGH); // INVERTED COZ OF THE PNP INSTEAD OF NPN TRANSISTOR
        Serial.println("R4 De-Energized (Console Command)");    
      }
    }
    {
      if (ch=='9'){
        digitalWrite (R1 ,HIGH);
        digitalWrite (R2 ,HIGH);
        digitalWrite (R3 ,HIGH); 
        digitalWrite (R4 ,LOW); // INVERTED COZ OF THE PNP INSTEAD OF NPN TRANSISTOR
        digitalWrite(12,HIGH);
        Serial.println("All Relays Energized (Console Command)");
      }
    }
    {
      if (ch=='0'){
        digitalWrite (R1,LOW);
        digitalWrite (R2,LOW); 
        digitalWrite (R3,LOW); 
        digitalWrite (R4,HIGH); // INVERTED COZ OF THE PNP INSTEAD OF NPN TRANSISTOR
        digitalWrite(12,LOW);

        Serial.println("All Relays De-Energized (Console Command)");
      }
    }
  }
}
