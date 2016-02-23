int flag = 0, RX_Flag = 0; // serial flag
char Code [14]; // read the serial port used to store data
long Num = 0; // decoded data
 
void Read_ID (void)
{
     int i = 0;
     char temp;
     for (i = 0; (Serial.available ()> 0); i ++) // serial idle
     while(Serial.available()>0)
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
 

 
void setup ()
{
   pinMode (13, OUTPUT);

   Serial.begin (9600);
   Serial.println ("This is a test for access control system");
   delay (100);

}
void loop ()
{
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
       Num <<= 4;
         if (Code [i]> 64) Num += ((Code [i]) - 55);
         else Num += ((Code [i]) - 48);
         Serial.println (Num);
     }
     Serial.println (Num);

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
