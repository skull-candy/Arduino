int flag = 0, RX_Flag = 0; // serial flag
long Num = 0; // decoded data
char Code [14]; // read the serial port used to store data
void Read_ID (void)
{
     int i = 0;
     char temp;
     for (i = 0; (Serial.available ()> 0); i ++) // serial idle
     //while(Serial.available()>0)
     {
       temp = Serial.read ();
       Serial.print (temp);
       delay (2);
       Serial.println (i);
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

void setup() {
   pinMode (13, OUTPUT);
   Serial.begin (9600);


}

void loop() {
  int i;
   long temp = 0, time = 0;
   RX_Flag = 0;
   while (1)
   {
    Read_ID ();
   if (RX_Flag == 1)
   {
     for (i = 5; i <11; i ++) // data decoding, six characters into hexadecimal number, the corresponding decimal card
     {


     }


   {
       digitalWrite (13, HIGH);
       delay (5);
       for (int i = 0; i <= 50; i ++) // give enough time to let it go to the steering angle is specified
      {

       }
       delay (2000);

       delay (1000);
       digitalWrite (13, LOW);
       delay (5);
       for (int i = 0; i <= 50; i ++) // give enough time to let it go to the steering angle is specified
   
 

     // Else fails Recognition
// {
// DigitalWrite (13, LOW);
// For (int i = 0; i <= 50; i ++) // give enough time to let it go to the steering angle is specified
// {
// Pulse (serpin, 90); // a reference pulse function
//}
//}
     while (Serial.read ()> 0); // Clear the cache
     RX_Flag = 0; // flag is cleared
     Num = 0; // data is cleared
    }
   }
   }
}



