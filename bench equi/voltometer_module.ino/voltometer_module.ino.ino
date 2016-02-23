const int voltPin = 5;
//Variables for voltage divider
float denominator;
int resistor1 = 3415;
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
        // voltage 2 - B (pin A3)
    Serial.print("B ");
    Serial.print(voltage);
    Serial.print("V ");
    // voltge 3 - C (pin A4)
    Serial.print("C ");
    Serial.print(voltage);
    Serial.print("V ");
    // voltage 4 - D (pin A5)
    Serial.print("D ");
    Serial.print(voltage);
    Serial.print("V ");
    Serial.println("");
    delay(10);
}

   
