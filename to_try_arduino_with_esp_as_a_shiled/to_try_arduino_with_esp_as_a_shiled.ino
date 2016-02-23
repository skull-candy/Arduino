#define BLYNK_PRINT Serial    // Comment this out to disable prints and save space
//#include <ESP8266_HardSer.h>
#include <BlynkSimpleShieldEsp8266_HardSer.h>
#include <Servo.h>
#include <SimpleTimer.h>
#define EspSerial Serial
ESP8266 wifi(EspSerial);
Servo servo;
SimpleTimer timer;

//SimpleTimer bridge;
WidgetBridge bridge1(V1);
char auth[] = "6fb37a42248c40eb84c1922aacca6555";
/////////////////////// Temperature Monitor Section Starts/////////////////////////
//// Temperature Related Reading
//int val;
//int tempPin = A0;
/////////////////////// Temperature Monitor Section ENDS///////////////////////////
void setup()
{
  servo.attach(11);
   servo.write(90);  // set servo to mid-point
   Serial.begin(115200);  // Set console baud rate
  delay(10);
   EspSerial.begin(115200);  // Set ESP8266 baud rate
  delay(10);
     timer.setInterval(2000L, sendUptime);
  Blynk.begin(auth, wifi, "GZ", "a4c100b3bb");
}
BLYNK_CONNECTED() {
  bridge1.setAuthToken("30a0c70e138a4b3aa3d0d4ce50b6afbe");
}
void loop()
{
  Blynk.run();
    timer.run();
}

void sendUptime()
{
  Blynk.virtualWrite(V0, millis() / 1000);
// val = analogRead(tempPin);
//float mv = (val/1024.0)*5000; 
//float cel = mv/10;
//
//Serial.print("TEMPRATURE = ");
//Serial.print(cel);
//Serial.print("*C");
//Serial.println();
     
 // Blynk.virtualWrite(V4, cel);
  bridge1.virtualWrite(V15, millis() / 1000);
 //   bridge1.virtualWrite(V18, cel);
}

void blynkAnotherDevice()
{
//   // bridge1.virtualWrite(V15, millis() / 1000);
//  bridge1.virtualWrite(V15, millis() / 1000);  
// // Blynk.virtualWrite(v2, 10, 20, 30);
  }
  
BLYNK_WRITE(V25) 
{
  int temp = param.asInt();
    Blynk.virtualWrite(V25, temp);
    
}

BLYNK_WRITE(V2)
{
  servo.write(180);  // set servo to mid-point
  delay(250);
  servo.write(90);  // set servo to mid-point
 }
 
BLYNK_WRITE(V5){
   int g = param.asInt(); //here is 44
    Blynk.virtualWrite(V5, g);
}
BLYNK_WRITE(V6){
   int h = param.asInt(); //here is 44
   if (h == 0) {
     
      servo.write(180);  // set servo to mid-point
  delay(250);
  servo.write(90);  // set servo to mid-point
   }
}
