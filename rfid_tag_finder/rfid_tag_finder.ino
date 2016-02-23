

int i;

void setup()
{
  Serial1.begin(9600);    // start serial to RFID reader
  Serial.begin(9600);  // start serial to PC 
}

void loop()
{
  if (Serial1.available() > 0) 
  {
     i = Serial1.read();
     Serial.print(i, OCT);
     Serial.print(" ");
  }
}
