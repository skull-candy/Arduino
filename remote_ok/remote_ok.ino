/*
  Digital Input / Output using standard programming techniques

 Connections:
 * Input A connected in pin A0
 * Input B connected in pin A1
 * Input C connected in pin A2
 * Input D connected in pin A3
 * Output W connected to pin Arduino 3
 * Output X connected to pin Arduino 5
 * Output Y connected to pin Arduino 6
 * Output Z connected to pin Arduino 9

*/

// Use constants to set pin numbers

const int inputA = A0;       // Input A
const int inputB = A1;       // Input B
const int inputC = A2;       // Input C
const int inputD = A3;       // Input D
const int outputW = 3;       // Output W
const int outputX = 4;       // Output X
const int outputY = 5;       // Output Y
const int outputZ = 6;       // Output Z

// Use variables for values that will change:
int readState = 0;           // Variable for reading the input status

void setup() {
  
  // Setup inputs:
  pinMode(inputA, INPUT);
  pinMode(inputB, INPUT);
  pinMode(inputC, INPUT);
  pinMode(inputD, INPUT);
  
  // Setup outputs:
  pinMode(outputW, OUTPUT);
  pinMode(outputX, OUTPUT);
  pinMode(outputY, OUTPUT);
  pinMode(outputZ, OUTPUT);
}

void loop() {
  // Read the state of the first input value:
  readState = digitalRead(inputA);

  if (readState == HIGH) {
    // turn LED on:
    digitalWrite(outputW, HIGH);
  }
  else {
    // turn LED off:
    digitalWrite(outputW, LOW);
  }
  
  // Read the state of the second input value:
  readState = digitalRead(inputB);

  if (readState == HIGH) {
    // turn LED on:
    digitalWrite(outputX, HIGH);
  }
  else {
    // turn LED off:
    digitalWrite(outputX, LOW);
  }
  
  // Read the state of the third input value:
  readState = digitalRead(inputC);

  if (readState == HIGH) {
    // turn LED on:
    digitalWrite(outputY, HIGH);
  }
  else {
    // turn LED off:
    digitalWrite(outputY, LOW);
  }
  
  // Read the state of the fourth input value:
  readState = digitalRead(inputD);

  if (readState == HIGH) {
    // turn LED on:
    digitalWrite(outputZ, HIGH);
  }
  else {
    // turn LED off:
    digitalWrite(outputZ, LOW);
  }
}
