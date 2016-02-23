#include <SPI.h>
#include <Ethernet.h>
#include <HttpClient.h>
#include <string.h>
#include <LiquidCrystal.h>
#include <SoftwareSerial.h>
 
//REST API settings
#define apiKey                "aa1bf85f3d7f43d9abd104df9ea16b4e" 
#define thingId               "12fc649716e511e49d127f574f86c83b"
#define methodId              "12a638de16e611e49d127f574f86c83b"
#define url                   "api.gadgetkeeper.com"
 
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(10, 128, 1, 222);
LiquidCrystal lcd(9, 8, 7, 6, 5, 4);
 
#define LOCK A0                       //mechanical LOCK power out pin
#define RED_LED A1                    //access denied LED
#define LOCK_OPEN_TIME 2000           //lock open time in milliseconds
 
//LCD messages
#define msgInit "Initializing.."
#define msgErrDhcp "DHCP Fail"
#define msgCon "Connecting.."
#define msgChkUsr "Validating User."
#define msgReady "Ready.."
#define msgNxt "Try Next card!"
#define msgChkFail "Check: Failed"
 
#define PORT  80              //web request port
String rfidTag = "";          //Ex: 0900B06F5A85
String userName ="";
char rfidChar;
 
EthernetClient client;
HttpClient http(client);
SoftwareSerial RFID(2, 3);    // RX, TX
 
void setup(){
    Serial.begin(9600);
    RFID.begin(9600);
    lcd.begin(16, 2);
    pinMode(LOCK, OUTPUT);
    pinMode(RED_LED, OUTPUT);
    digitalWrite(RED_LED, LOW);
    digitalWrite(LOCK, HIGH);
    Serial.println(msgInit);  //start the Ethernet connection:
    lcd.print(msgInit);
    if (Ethernet.begin(mac) == 0){
        Serial.println(F("Failed to configure Ethernet using DHCP"));
        lcd.clear();
        lcd.print(msgErrDhcp);
        Ethernet.begin(mac, ip);
    }
    delay(1000);  //give the Ethernet shield a second to initialize:
    lcd.clear();
    lcd.print(msgReady);
    Serial.println(msgReady);
    digitalWrite(RED_LED, HIGH);
}
 
//validate user
int getUserById(String tag){
  userName = "";
  tag = "\"" + tag + "\"";
  char msgBuffer[150], valueBuffer[18], msgContentLength = 0;
  int result;    
  memset(msgBuffer , 0 , 150);
  sprintf(msgBuffer , "/v1/things/%s/methods/%s/execute.json" , thingId, methodId);
  http.beginRequest();
  result = http.startRequest(url, PORT, msgBuffer, HTTP_METHOD_PUT, "Arduino");
  if(result == HTTP_ERROR_API){
      Serial.println(F("API error"));
      http.endRequest();        
      http.stop();
      return 1;        
  }else if(result == HTTP_ERROR_CONNECTION_FAILED){
      Serial.println(F("Connection failed"));    
      http.endRequest();    
      http.stop();
      return 1;                        
  }else if( result == HTTP_SUCCESS ){
      memset( msgBuffer , 0 , 80 );
      sprintf( msgBuffer , "X-ApiKey: %s" , apiKey );
      http.sendHeader(msgBuffer);
      http.sendHeader("Content-Type: text/json; charset=UTF-8");
      tag.toCharArray(valueBuffer, 18);
      valueBuffer[strlen(valueBuffer)] = '\r';
      valueBuffer[strlen(valueBuffer)+1] = '\n';
      valueBuffer[strlen(valueBuffer)+2] = '\r';
      valueBuffer[strlen(valueBuffer)+3] = '\n';
      msgContentLength = strlen(valueBuffer) - 1 ;
      memset(msgBuffer , 0 , 80);
      sprintf( msgBuffer , "Content-Length: %d" , msgContentLength );
      http.sendHeader(msgBuffer);
      http.write((const uint8_t*) valueBuffer , msgContentLength + 2 );
      http.endRequest();
      int err =0;
      err = http.responseStatusCode();
      if(err < 0){
        http.stop();
        return 1; 
      }
      err = http.skipResponseHeaders();
      char c;
      int count = 0;
      boolean saveNow =false;
      while(1){
        c = http.read();
        //Serial.print(c);
        if(c == '"'){
          saveNow = true;
          count++;
        }else if(saveNow && count < 2){
          userName = userName + c;
        }else if(count >= 2){
          break;
        }  
      }
      lcd.clear();
      lcd.print(msgChkUsr);
      Serial.print(F("Sending tag : "));                        
      Serial.println(tag);
      Serial.print(F("User : "));
      Serial.println(userName);
      http.stop();        
      return 0;
  }else{
      http.endRequest();    
      http.stop();                                    
      return 1;
  }        
}
 
void rfid_flush(){
    while(RFID.available() > 0)
        RFID.read(); 
}
 
void loop(){
  int progress;
  while(RFID.available()>0){
    rfidChar = RFID.read();
    if((rfidChar - '0') == -46){
      rfidTag = "";
    }else if((rfidChar - '0') != -45){
      rfidTag += rfidChar;
    }else if((rfidChar - '0') == -45){
      Serial.println("connecting...");
      lcd.clear();
      lcd.print(msgCon);
      progress = getUserById(rfidTag);
      Serial.print("Validating user: ");
      if(!progress){
          Serial.println("OK ");
      }else{
          Serial.println(F("Fail")); 
          lcd.clear();
          lcd.print(msgChkFail); 
      } 
      Serial.print(rfidTag);
      Serial.print(": ");
      Serial.println(userName);
       
      if(userName != "Unknown" && userName != ""){
          digitalWrite(LOCK, LOW);
          lcd.setCursor(0, 1);
          lcd.print(F("Hi: "));
          lcd.setCursor(4, 1);
          lcd.print(userName);
      }else{
          digitalWrite(RED_LED, LOW);
          lcd.setCursor(4, 1);
          lcd.print(userName);  
      }
      delay(LOCK_OPEN_TIME);
      digitalWrite(LOCK, HIGH);
      digitalWrite(RED_LED, HIGH);
      lcd.clear();
      lcd.print(msgNxt);
      rfid_flush();
      break;
    }
  }
}
