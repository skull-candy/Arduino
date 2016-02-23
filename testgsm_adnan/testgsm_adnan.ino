void setup() 
{
  Serial.begin(9600);
  Serial1.begin(9600);
  delay(1000);
  Serial.println("Initializing...");

}

void loop()  
{
  if(Serial.available())
  {
    char a=Serial.read();
    Serial1.print(a);
//    Serial.print(a);
  }
  
  if(Serial1.available())
  {
    char b=Serial1.read();
    Serial.print(b);
  }
  delay(100);
}

