// Sketch by Ahsan Muhammad to Controll Two Relays By SERIAL COMMANDS AND A IR REMOTE 
// V.3.0 Date : 15-11-13
#include <IRremote.h>
int IR_LED = 11;
int R1 = 9;
int R2 = 10;
int R1_LED = 13;
int R2_LED = 12;
int R3 = 8;
int R4 = 7;
int on = 0;

IRrecv irrecv(IR_LED);
decode_results results;

void setup()
{
    pinMode(R1, OUTPUT);
    pinMode(R2, OUTPUT);
    pinMode(R3, OUTPUT);
    pinMode(R4, OUTPUT);
    pinMode(R1_LED, OUTPUT);
    pinMode(R2_LED, OUTPUT);
    pinMode(IR_LED, INPUT) ;
   
      
      Serial.begin(9600);      
      irrecv.enableIRIn(); // Start the receiver
  } 
unsigned long last = millis();
void loop() 
 {
   char ch = Serial.read();
   if (ch=='1'){
     digitalWrite (R1 ,HIGH);
     digitalWrite(13, HIGH);
    Serial.println("R1 Energized (Console Command)");  
   }
   {
   if (ch=='2'){
     digitalWrite (R1 ,LOW);
   digitalWrite(13,  LOW);
    Serial.println("R1 De-Energized (Console Command)");    
   }
    {
   if (ch=='3'){
     digitalWrite (R2 ,HIGH);
   digitalWrite(12,HIGH);
    Serial.println("R2 Energized (Console Command)");    
   }
    {
   if (ch=='4'){
     digitalWrite (R2 ,LOW);
   digitalWrite(12,LOW);
    Serial.println("R2 De-Energized (Console Command)");    
   }
     {
   if (ch=='5'){
     digitalWrite (R3 ,HIGH);
     
    Serial.println("R3 Energized (Console Command)");    
   }  {
   if (ch=='6'){
     digitalWrite (R3 ,LOW);
    Serial.println("R3 De-Energized (Console Command)");    
   }
     {
   if (ch=='7'){
     digitalWrite (R4 ,LOW);
    Serial.println("R4 Energized (Console Command)");    
   }  {
   if (ch=='8'){
     digitalWrite (R4 ,HIGH);
    Serial.println("R4 De-Energized (Console Command)");    
   }
   {
   if (ch=='9'){
     digitalWrite (R1 ,HIGH);
   digitalWrite (R2 ,HIGH);
     digitalWrite (R3 ,HIGH); 
        digitalWrite (R4 ,LOW);
  digitalWrite(12,HIGH);
 digitalWrite(13,HIGH); 
 Serial.println("All Relays Energized (Console Command)");
   }
    {
   if (ch=='0'){
     digitalWrite (R1,LOW);
   digitalWrite (R2,LOW); 
   digitalWrite (R3,LOW); 
   digitalWrite (R4,HIGH); 
  digitalWrite(12,LOW);
 digitalWrite(13,LOW); 
 Serial.println("All Relays De-Energized (Console Command)");
   }
{
  if (irrecv.decode(&results)) {
    if (results.value == 0xFF30CF) { // 1 button for R1
      if (millis() - last > 200) {
        on = !on; // IR received, toggle the relay
        digitalWrite(R1, on ? HIGH : LOW);
        digitalWrite(13, on ? HIGH : LOW);
    Serial.println("R1 toggled (Remote Command)");  
      }
      last = millis();
    }    
    irrecv.resume(); // Receive the next value
  }{
    if (irrecv.decode(&results)) {
    if (results.value == 0xFF18E7) { // 2 button for R2
      if (millis() - last > 200) {
        on = !on; // IR received, toggle the relay
        digitalWrite(R2, on ? HIGH : LOW);
        digitalWrite(12, on ? HIGH : LOW);
    Serial.println("R2 toggled (Remote Command)");  
      }
      last = millis();
    }}}    
    irrecv.resume(); // Receive the next value
  }{
   if (irrecv.decode(&results)) {
    if (results.value == 0xFFB04F) { // FL- button to deactivate every relay
      if (millis() - last > 200) {
        on = !on; // IR received, toggle the relay
        digitalWrite(R1, LOW);
        digitalWrite(R2, LOW);
        digitalWrite(R1_LED, LOW);
        digitalWrite(R2_LED, LOW);
    Serial.println("All Relays De-Energized (Remote Command)");  
      }
      last = millis();
    }    
    irrecv.resume(); // Receive the next value
   }{
       if (irrecv.decode(&results)) {
    if (results.value == 0xFF9867) { // FL+ button to activate every relay
      if (millis() - last > 200) {
        on = !on; // IR received, toggle the relay
        digitalWrite(R1, HIGH);
        digitalWrite(R2, HIGH);
        digitalWrite(R1_LED, HIGH);
        digitalWrite(R2_LED, HIGH);
    Serial.println("All Relays Energized (Remote Command)");  
      }
      last = millis();
    }}}}    
    irrecv.resume(); // Receive the next value
    }}}}}}}}}}
