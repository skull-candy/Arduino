//
//----- BACK DOOR RFID ARDUINO SKETCH -----
//----- WRITTEN BY BILL PHILLIPS - jeepdude48507 -----
//----- INITIALIZE HARDWARE VARIABLES -----
int Invalid = 7; //----- INVALID READ OUTPUT LED (ORANGE) -----
int P_Button = 14; //----- PUSH BUTTON INPUT PIN 6 -----
int Read = 6; //----- RFID GOOD READ LED (GREEN) -----
int Beeper_Pin = 12; //----- BEEPER OUTPUT PIN -----
int Relay_A = 15; //----- Relay_A OUTPUT PIN - OPEN -----
int Relay_B = 14; //----- Relay_B OUTPUT PIN - CLOSE -----
int Locked = 0; //----- DEADBOLT LOCKED LED (RED) -----
int UnLocked = 6; //----- DEADBOLT OPEN LED (GREEN) -----
int ValidRead = 6; //----- VALID READ OUTPUT LED (GREEN) -----
int A_Delay = 5000; //----- RELAY A ON TIME -----
int B_Delay = 60000; //----- RELAY B ON TIME -----
int Toggle_Flag = 1; //----- OPEN / CLOSE FLAG -----

//----- INITIALIZE SOFTWARE VARIABLES -----
int Beep_Count = 0; //----- NUMBER OF BEEPS -----
int z = 0;
int i = 0; //----- JUNK VARIABLE -----
long Idle_Count = 0; //----- IDLE TIME COUNTER -----
int Index = 0; //----- COUNTS EACH BYTE READ FROM RFID MODULE -----
int BadgeRead[14]; //----- STORES BYTES READ FROM RFID MODULE INTO AN ARRAY -----
int match1 = 0; //----- RFID TAG 1 MATCH COUNTER -----
int match2 = 0; //----- RFID TAG 2 MATCH COUNTER -----
int match3 = 0; //----- RFID TAG 3 MATCH COUNTER -----
int match4 = 0; //----- RFID TAG 4 MATCH COUNTER -----
int match5 = 0; //----- RFID TAG 5 MATCH COUNTER -----
int match6 = 0; //----- RFID TAG 6 MATCH COUNTER -----
int match7 = 0; //----- RFID TAG 7 MATCH COUNTER -----
int match8 = 0; //----- RFID TAG 8 MATCH COUNTER -----
int match9 = 0; //----- RFID TAG 9 MATCH COUNTER -----
int flag = 0;   //----- FLAG VALID TAG MATCH -----

//----- SETUP VALID BADGES -----
// WHEN YOU SCAN YOUR FIRST FOB OR CARD MAKE SURE SERIAL MONOTOR IS RUNNING. AFTER SCANNING
// READ THE NUMBER THAT APPEARS IN THE MONITOR. COPY THIS NUMBER TO ONE OF THE SECTIONS BELOW
// AND MODIFY THE COMMENT AT THE END FOR FUTURE REFERENCE. I HAVE FOUND IT NECESSARY TO ADD
// AND REMOVE FOBS I HAVE GIVEN OUT. WITH THIS METHOD YOU CONTROL WHO HAS ACCESS TO YOUR
// HOME AND UNLIKE A KEY, THESE ARE VERY HARD TO HACK. IT IS CONVIENENT TO ADD THE NUMBER
// PRINTED ON THE FOB OR CARD FOR EASY REFERENCE. 
// IN THIS SKETCH EACH FOB IS REFERED TO AS BADGE1, BADGE2, BADGE3, ETC.

int Badge1[14] = {2, 53, 51, 48, 48, 49, 55, 65, 69, 51, 53, 68, 70, 3};// BADGE1 - YELLOW FOB - SPARE - BADGE1 - 0012345678 <---- NUMBER PRINTED ON FOB
int Badge2[14] = {2, 65, 63, 60, 60, 61, 67, 101, 121, 246, 23, 31, 15  };// BADGE2 - WHITE CARD #3 - SPARE - BADGE2
int Badge3[14] = {2, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 3};// BADGE3 - RED FOB - SPARE - BADGE3
int Badge4[14] = {2, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 3};// BADGE4 - BLUE FOB - SPARE - BADGE4
int Badge5[14] = {2, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 3};// BADGE5 - WHITE CARD #4 - BADGE5
int Badge6[14] = {2, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 3};// BADGE6 - WHITE CARD #5 - BADGE6
int Badge7[14] = {2, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 3};// BADGE7 - RED FOB - BADGE7
int Badge8[14] = {2, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 3};// BADGE8 - BLUE FOB - SPARE - BADGE8 - 0012348765 
int Badge9[14] = {2, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 3};// BADGE9 - YELLOW - YELLOW FOB - SPARE - 0012121212

//------------------------------------------------------------------------------------------------
void setup() {
  //----- START SERIAL COMMUNICATION -----  
  Serial.begin(9600);
    Serial1.begin(9600); 
  //----- SET PINMODES -----
  pinMode(Locked, OUTPUT);                       //----- INVALID RED LED TO OUTPUT PIN 11 -----
  pinMode(UnLocked, OUTPUT);                     //----- INVALID GREEM LED TO OUTPUT PIN 12 ----- 
  pinMode(P_Button, INPUT);                      //----- PUSH BUTTON FROM INPUT PIN 6 -----   
  pinMode(Relay_A, OUTPUT);                      //----- RELAY OUTPUT TO PIN 9 -----
  pinMode(Relay_B, OUTPUT);                      //----- RELAY OUTPUT TO PIN 10 -----
  pinMode(Beeper_Pin, OUTPUT);                   //----- Beeper_Pin OUTPUT TO PIN 13 -----

  //----- BEEP 3 TIMES ON STARTUP -----   
  Beep_Count = 3; 
  beepit();
}//----- VOID SETUP BRACKET -----

//------------------------------------------------------------------------------------------------   
void loop() {

  //----- MANUAL DEADBOLT TOGGLE WHEN BUTTON PUSHED AT PIN 6 -----
  if (digitalRead(P_Button) == HIGH) { 
    Toggle();                                //----- CALL TOGGLE ROUTINE -----
    Index = 0;                              //----- RESET ALL DATA FOR NEXT READ -----
    Serial.println(" -- BUTTON AT PIN 6 PRESSED");
    }

  //----- READ FROM RFID MODULE -----  
  if(Serial1.available() > 0) {
    BadgeRead[Index] = Serial.read();
    
  //----- COMPARE BADGE1 BYTE TO READ BYTE -----
    if(Badge1[Index] == BadgeRead[Index]) { 
      match1 = match1 + 1; 

  } 
    //----- COMPARE BADGE2 BYTE TO READ BYTE -----
    if(Badge2[Index] == BadgeRead[Index]) { 
      match2 = match2 + 1;
    }  
 
    //----- COMPARE BADGE3 BYTE TO READ BYTE -----
    if(Badge3[Index] == BadgeRead[Index]) { 
      match3 = match3 + 1; 
    
    }      
    //----- COMPARE BADGE4 BYTE TO READ BYTE -----
    if(Badge4[Index] == BadgeRead[Index]) { 
      match4 = match4 + 1; 
    } 
    //----- COMPARE BADGE5 BYTE TO READ BYTE -----
    if(Badge5[Index] == BadgeRead[Index]) { 
      match5 = match5 + 1; 
    }      
    //----- COMPARE BADGE6 BYTE TO READ BYTE -----
    if(Badge6[Index] == BadgeRead[Index]) { 
      match6 = match6 + 1; 
    }  
    //----- COMPARE BADGE6 BYTE TO READ BYTE -----
    if(Badge7[Index] == BadgeRead[Index]) { 
      match7 = match7 + 1; 
    }  
    //----- COMPARE BADGE6 BYTE TO READ BYTE -----
    if(Badge8[Index] == BadgeRead[Index]) { 
      match8 = match8 + 1; 
    }  
    //----- COMPARE BADGE6 BYTE TO READ BYTE -----
    if(Badge9[Index] == BadgeRead[Index]) { 
      match9 = match9 + 1; 
    }      

    //----- SEND READ BYTE TO DISPLAY -----     
    Serial.print(BadgeRead[Index]);

    //----- TEST FOR VALID BADGE1 -----
    if(Index == 13 && match1 == 14) {
      flag = 1;
      Toggle();                                  //----- OPEN OR CLOSE DEADBOLT -----
      digitalWrite(Relay_A, HIGH);               //----- TURN Relay_A ON AT PIN 9 -----
      delay(A_Delay);                                //----- HOLD IT ON 1/4 SECOND -----
      digitalWrite(Relay_A, LOW);                //----- TURN Relay_A OFF AT PIN 9 -----             
      Serial.print("  YELLOW FOB - SPARE - BADGE1");
    }   

    //----- TEST FOR VALID BADGE2 -----
    if(Index == 13 && match2 == 14) {
      flag = 1;
      Toggle();                                  //----- OPEN OR CLOSE DEADBOLT -----
      Serial.print("  WHITE CARD #3 - SPARE - BADGE2");
    }       

    //----- TEST FOR VALID BADGE3 -----
    if(Index == 13 && match3 == 14) {
      flag = 1;
      Toggle();                                  //----- OPEN OR CLOSE DEADBOLT -----
      Serial.print("  RED FOB - SPARE - BADGE3");
    }      

    //----- TEST FOR VALID BADGE4 -----
    if(Index == 13 && match4 == 14) {
      flag = 1;
      Toggle();                                  //----- OPEN OR CLOSE DEADBOLT -----
      Serial.print("  BLUE FOB - SPARE - BADGE4");
    }   

    //----- TEST FOR VALID BADGE5 -----
    if(Index == 13 && match5 == 14) {
      flag = 1;
      Toggle();                                  //----- OPEN OR CLOSE DEADBOLT -----
      Serial.print("  WHITE CARD #4 - BADGE5");
    }  

    //----- TEST FOR VALID BADGE6 -----
    if(Index == 13 && match6 == 14) {
      flag = 1;
      Toggle();                                  //----- OPEN OR CLOSE DEADBOLT -----
      Serial.print("  WHITE CARD #5 - BADGE6");
    }   
    
    //----- TEST FOR VALID BADGE7 -----
    if(Index == 13 && match7 == 14) {
      flag = 1;
      Toggle();                                  //----- OPEN OR CLOSE DEADBOLT -----
      Serial.print("  RED FOB - BADGE7");
    }    
    
    //----- TEST FOR VALID BADGE8 -----
    if(Index == 13 && match8 == 14) {
      flag = 1;
      Toggle();                                  //----- OPEN OR CLOSE DEADBOLT -----
      Serial.print("  BLUE FOB - SPARE - BADGE8");
    }  
  
    //----- TEST FOR VALID BADGE9 -----
    if(Index == 13 && match9 == 14) {
      flag = 1;
      Toggle();                                  //----- OPEN OR CLOSE DEADBOLT -----
      Serial.print("  SPARE YELLOW FOB - BADGE9");
    }

    //----- DOUBLE BEEP ON VALID BADGE ON PIN 11 -----      
    if(Index == 13 && flag == 1) {
      digitalWrite(Read, HIGH);                //----- TURN GOOD READ LED ON AT PIN 7 -----
      Beep_Count = 2; 
      beepit(); 
      digitalWrite(Read, LOW);                 //----- TURN GOOD READ LED OFF AT PIN 7 -----
    }

    //----- TEST FOR INVALID BADGE ----- 
    if(Index == 13 && flag == 0) {
      Serial.print("  UNAUTHORIZED");   
      digitalWrite(Read, HIGH);                 //----- TURN GOOD READ LED ON AT PIN 7 -----
      digitalWrite(Invalid, HIGH);              //----- TURN INVALID LED ON AT PIN 8 -----
      digitalWrite(Beeper_Pin, HIGH);           //----- TURN Beeper_Pin ON AT PIN 13 -----
      delay(1000);                              //----- HOLD LED AND BEEP FOR 1 SECOND -----
      digitalWrite(Invalid, LOW);               //----- TURN INVALID LED OFF AT PIN 8 ----- 
      digitalWrite(Beeper_Pin, LOW);            //----- TURN Beeper_Pin OFF AT PIN 13 -----
      digitalWrite(Read, LOW);                  //----- TURN GOOD READ LED OFF AT PIN 7 -----
      } 

    //----- INCREMENT BYTE READ COUNTER ----- 
    Index = Index + 1; //----- INCREMENT COUNTER -----

    //----- END OF DATA STRING INDICATOR FOR SERIAL DISPLAY -----
    if(Index > 13) { 
      Serial.println(" ");    
      Serial.println("---------------------------------------");
      Index = 0; 
      flag = 0;
      match1 = 0;
      match2 = 0;
      match3 = 0;
      match4 = 0;
      match5 = 0;
      match6 = 0; 
      match7 = 0;
      match8 = 0;
      match9 = 0;
    }
    else {
      Serial.print(", "); 
    }  
  }  
}//----- VOID LOOP BRACKET -----

//----- BEEP Beep_Count TIMES -----
int beepit(){
  for(i=0; i<Beep_Count; i++) {
    digitalWrite(Beeper_Pin, HIGH);             // TURN Beeper_Pin ON AT PIN 11 FOR 80 MILLISECONDS -----
    delay(120);
    digitalWrite(Beeper_Pin, LOW);              //----- TURN Beeper_PinBeep_Count OFF AT PIN 11 FOR 50 MILLISECONDS -----
    delay(50); 
  } 
}

  //----- OPEN THE DEADBOLT -----
  int Toggle() {
  if(Toggle_Flag == 1) { 
    Serial.println("  Toggle Flag = 1 / OPEN DEADBOLT");
    digitalWrite(Relay_A, HIGH);             //----- TURN Relay_A ON AT PIN 9 -----
    delay(A_Delay);                          //----- HOLD A WHILE -----
    digitalWrite(Relay_A, LOW);              //----- TURN Relay_A OFF AT PIN 9 ----- 
    digitalWrite(UnLocked, HIGH);            //----- TURN OPEN GREEN LED ON AT PIN 12 -----
    digitalWrite(Locked, LOW);               //----- TURN LOCKED LED OFF AT PIN 11 -----
  }
  //----- CLOSE THE DEADBOLT -----
  if(Toggle_Flag == -1) { 
    Serial.println("  Toggle Flag = -1 / CLOSE DEADBOLT");
    digitalWrite(Relay_B, HIGH);             //----- TURN Relay_A ON AT PIN 10 -----
    delay(A_Delay);                          //----- HOLD A WHILE -----
    digitalWrite(Relay_B, LOW);              //----- TURN Relay_A OFF AT PIN 10 ----- 
    digitalWrite(UnLocked, LOW);             //----- TURN OPEN GREEN LED OFF AT PIN 12 -----
    digitalWrite(Locked, HIGH);              //----- TURN LOCKED LED ON AT PIN 11 -----    
  } 
  Toggle_Flag = Toggle_Flag * -1; 
 }


/* 
      Serial.println(" ");    
      Serial.println("----------------------------------------------------------------------------");
      
      Serial.print("Index="); 
      Serial.print(Index);

      Serial.print("  BadgeRead[Index]=");
      Serial.print(BadgeRead[Index]);

      Serial.print("  Badge2[Index]=");
      Serial.print(Badge2[Index]);

      Serial.print("  match2=");
      Serial.println(match2);
      
      Serial.println(" ");    
      Serial.println("----------------------------------------------------------------------------");
*/ 
