

char RFID;
void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  Serial1.begin(9600);
}
void loop()
{

  if (Serial1.available() > 0) {// send data only when you receive data:

    RFID = Serial1.read();// read the incoming byte:
  
   //////////////
    Serial.print(RFID);//Print Data
    //delay(10);
  }
  
}

