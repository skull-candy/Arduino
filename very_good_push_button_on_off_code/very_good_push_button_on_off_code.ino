/* sketch 3 
turn on a LED when the button is pressed and let it on 
until the button is pressed again
*/
int pinButton = 31;
int LED = 6;
int stateLED = HIGH;
int stateButton;
int previous = LOW;
long time = 0;
long debounce = 500;
 
void setup() {
  pinMode(pinButton, INPUT);
  pinMode(LED, OUTPUT);
}
 
void loop() {
  stateButton = digitalRead(pinButton);  
  if(stateButton == HIGH && previous == LOW && millis() - time > debounce) {
    if(stateLED == HIGH){
      stateLED = LOW; 
    } else {
       stateLED = HIGH; 
    }
    time = millis();
  }
  digitalWrite(LED, stateLED);
  previous == stateButton;
}
