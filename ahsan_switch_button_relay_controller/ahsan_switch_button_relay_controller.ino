int pinButton = 8;
int Relay = 2;
int stateRelay = HIGH;
int stateButton;
int previous = LOW;
long time = 0;
long debounce = 1000;
 
void setup() {
  pinMode(pinButton, INPUT);
  pinMode(Relay, OUTPUT);
}
 
void loop() {
  stateButton = digitalRead(pinButton);  
  if(stateButton == HIGH && previous == LOW && millis() - time > debounce) {
    if(stateRelay == HIGH){
      stateRelay = LOW; 
    } else {
       stateRelay = HIGH; 
    }
    time = millis();
  }
  digitalWrite(Relay, stateRelay);
  previous == stateButton;
}
