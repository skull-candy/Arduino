#include <SPI.h>
#include <Ethernet.h>


int flag = 0, RX_Flag = 0; // serial flag
char Code [14]; // read the serial port used to store data
long Num = 0; // decoded data
int i;
long temp = 0, time = 0;
void Read_ID (void)
{
  int i = 0;
  char temp;
  for (i = 0; (Serial.available () > 0); i ++) // serial idle
    while (Serial.available() > 0)
    {
      temp = Serial.read ();
      Serial.print (temp);
      delay (2);
      // Serial.println (i);
      if (temp == 0X02) // receives the start bit
      {
        flag = 1; i = 0; RX_Flag = 0; //
      }
      if (flag == 1) // start bit is detected, begin receiving data
      {
        if (temp == 0X03) // end code is detected,
        {
          flag = 0; // flag is cleared
          if (i == 13) RX_Flag = 1; // first 13 yards to the end, receive data, a flag is set
          else RX_Flag = 0;
          break;
        }
        Code [i] = temp;
      }
    }
  flag = 0; // flag is cleared
}

///////ETHERNET CODE STARTS HERE///////////////////

byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xEE
};
IPAddress ip(192, 168, 0, 250);

EthernetServer server(80);
//////////////ETHERNET CODE ENDS HERE////////////
void setup ()
{
  pinMode (13, OUTPUT);

  Serial.begin (9600);
  Serial.println ("This is a test for access control system");
  delay (100);
  ////////////////ETHERNET CODE STARTS HERE//////////////
  
  // Open serial communications and wait for port to open:

  // start the Ethernet connection and the server:
  Ethernet.begin(mac, ip);
  server.begin();
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());
  
  ////////////////ETHERNET CODE ENDS HERE////////////////
}
void loop ()
{
  ///////////////////////ETHERNET CODE STARTS HERE////////////////
  // listen for incoming clients
  EthernetClient client = server.available();
  if (client) {
    Serial.println("new client");
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {
          // send a standard http response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");  // the connection will be closed after completion of the response
          client.println("Refresh: 5");  // refresh the page automatically every 5 sec
          client.println();
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");
          //        output the value of each analog input pin
          for (int analogChannel = 0; analogChannel < 6; analogChannel++) {
            int sensorReading = analogRead(analogChannel);

            client.print(temp);
            client.println("<br />");
          }
          client.println("</html>");
          break;
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        }
        else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    // give the web browser time to receive the data
    delay(1);
    // close the connection:
    client.stop();
    Serial.println("client disconnected");
  }

  ////////////////////////ETHERNET CODE ENDS HERE/////////////////
  int i;
  long temp = 0, time = 0;
  RX_Flag = 0;
  while (1)
  {
    Read_ID ();
    if (RX_Flag == 1)
    {
      for (i = 5; i < 11; i ++) // data decoding, six characters into hexadecimal number, the corresponding decimal card
      {
        Num <<= 4;
        if (Code [i] > 64) Num += ((Code [i]) - 55);
        else Num += ((Code [i]) - 48);
        Serial.println (Num);
      }
      Serial.println (Num);

      while (Serial.read () > 0); // Clear the cache
      RX_Flag = 0; // flag is cleared
      Num = 0; // data is cleared
    }
  }


}

