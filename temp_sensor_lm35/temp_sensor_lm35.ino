int val;
int tempPin = 0;
int centi = 0;
void setup()
{
  Serial.begin(9600);
}
void loop()
{
  val = analogRead(tempPin);
  float mv = ( val/1024.0)*5000; 
  float cel = mv/10;
  float farh = (cel*9)/5 + 32; // 32
  float centi =(cel-2);

  Serial.print("TEMPRATURE = ");
  Serial.print(centi);
  Serial.print("*C");
  Serial.println();
  delay(50);

/*  uncomment this to get temperature in farenhite 
  Serial.print("TEMPRATURE = ");
  Serial.print(farh);
  Serial.print("*F");
  Serial.println();


*/
}

