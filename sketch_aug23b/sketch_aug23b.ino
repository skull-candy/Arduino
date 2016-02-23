
if (mySerial.available())
{
   Serial.write(mySerial.read());
}
 
 
if (mySerial.available() > 0)
{
  Serial.println(mySerial.read());
}
 
 
if(mySerial.available() >0)
  {
    incoming_char=mySerial.read(); //Get the character from the cellular serial port.
    Serial.print(incoming_char); //Print the incoming character to the terminal.
  }
}
 
 
Complete code:
 
#include <SoftwareSerial.h>
 
 
SoftwareSerial mySerial(19, 18);  //Arduino 18 pin(TX1) and 19 pin(RX1)
 
 
int LED = 13;
 
 
void setup()
{
pinMode(LED, OUTPUT);
Serial.begin(19200);
mySerial.begin(19200);  // the GPRS baud rate
powerUp();
 
}
 
void powerUp()
{
pinMode(9, OUTPUT);
digitalWrite(9,LOW);
delay(1000);
digitalWrite(9,HIGH);
delay(2000);
digitalWrite(9,LOW);
delay(9000);
Serial.println("Modem powered");
}
 
void loop()
{
  if (Serial.available())
  {
    char inSer = Serial.read();
    switch (inSer)
    {
      case '1':      //Just for confirmation that incoming characters from serial port is reading
      digitalWrite(LED, HIGH);
      delay(3000);
      digitalWrite(LED, LOW);
      break;
 
      case '2':
      mySerial.println("ATD + +371200....;"); //Calling to my cell. I just deleted part of my phone number cus i dont want to shove it
      delay(100);
      mySerial.println();
      delay(7000);       
      mySerial.println("ATH");  //Hangup
      break;
 
      case '3':
      mySerial.println("AT+CNMI=?\r");  //asking for new sms message indication
      delay(100);
      break;
 
      case '4':
      mySerial.println("AT+CSQ=?\r");  //asking for signal quality report
      delay(100);
      break;
    }
  }
if (mySerial.available())
{
   Serial.write(mySerial.read());
}
}
 
