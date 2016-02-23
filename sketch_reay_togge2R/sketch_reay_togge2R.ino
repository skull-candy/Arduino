
#include <IRremote.h>
int RECV_PIN = 11;
int OUTPUT_PIN = 4;
int OUTPUT_PIN2 = 5;

IRrecv irrecv(RECV_PIN);
decode_results results;
void setup()
{
  pinMode(OUTPUT_PIN, OUTPUT);
    pinMode(OUTPUT_PIN2, OUTPUT);
  pinMode(13, OUTPUT);
      Serial.begin(9600);
  irrecv.enableIRIn(); // Start the receiver
}
int on = 0;
unsigned long last = millis();
void loop() {
  if (irrecv.decode(&results)) {
    if (results.value == 0xFFA25D) { // ch- button
      if (millis() - last > 250) {
        on = !on; // IR received, toggle the relay
        digitalWrite(OUTPUT_PIN, on ? HIGH : LOW);
        digitalWrite(13, on ? HIGH : LOW);
    Serial.println("R1 toggled");  
      }
      last = millis();
    }    
    irrecv.resume(); // Receive the next value
  }
    if (irrecv.decode(&results)) {
    if (results.value == 0xFF30CF) { // ch- button
      if (millis() - last > 250) {
        on = !on; // IR received, toggle the relay
        digitalWrite(OUTPUT_PIN2, on ? HIGH : LOW);
        digitalWrite(13, on ? HIGH : LOW);
    Serial.println("R2 toggled");  
      }
      last = millis();
    }    
    irrecv.resume(); // Receive the next value
  }
}

