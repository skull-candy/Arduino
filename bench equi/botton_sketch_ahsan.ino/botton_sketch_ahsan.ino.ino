const int voltPin = 5;
//Variables for voltage divider
float denominator;
int resistor1 = 3405;
int resistor2 = 1050;
void setup() {
    Serial.begin(9600); 

}

void loop() {
 float voltage;
int V = voltage;
denominator = (float)resistor2 / (resistor1 + resistor2);
 //Obtain RAW voltage data
  voltage = analogRead(voltPin);
  //Convert to actual voltage (0 - 5 Vdc)
  voltage = (voltage / 2048) * 10 ;
  //Convert to voltage before divider
  //  Divide by divider = multiply
  //  Divide by 1/5 = multiply by 5
  voltage = voltage / denominator;
  //Output to serial
 Serial.print("A ");
    Serial.print(voltage);
    Serial.print("V ");
    Serial.println("");
    delay(100);
}

   
