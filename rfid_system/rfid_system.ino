
//Sparkfun RFID KIT TX connected to Arduino pin 0 RX,

char RFID;

void setup() {
	Serial.begin(9600);	// opens serial port, sets data rate to 9600 bps
             }

void loop() {

if (Serial.available() > 0) {// send data only when you receive data:
RFID = Serial.read();// read the incoming byte:
Serial.print(RFID);//Print Data
    }
  }
