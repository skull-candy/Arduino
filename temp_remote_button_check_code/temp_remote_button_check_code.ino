//Button_01
//Control LED on and off by a push button
//
// created by uvvvvw
// 2015-04-05
//
const int Button = 31; //Define button pin
const int LED = 5; //Define LED pin
int LED_state = LOW; //Status of LED : LOW(on), HIGH(off)


const int Button2 = 33; //Define button pin
const int LED2 = 7; //Define LED pin
int LED_state2 = LOW; //Status of LED : LOW(on), HIGH(off)


const int Button3 = 35; //Define button pin
const int LED3 = 6; //Define LED pin
int LED_state3 = LOW; //Status of LED : LOW(on), HIGH(off)


void setup()
{
pinMode(Button, INPUT);
digitalWrite(Button, HIGH); //Use internal pull up resistor for the button. It saved a resistor for the button.
// 
pinMode(Button2, INPUT);
digitalWrite(Button2, HIGH); //Use internal pull up resistor for the button. It saved a resistor for the button.

pinMode(Button3, INPUT);
digitalWrite(Button3, HIGH); //Use internal pull up resistor for the button. It saved a resistor for the button.

pinMode( LED, OUTPUT);
digitalWrite( LED, HIGH);

pinMode( LED2, OUTPUT);
digitalWrite( LED2, HIGH);

pinMode( LED3, OUTPUT);
digitalWrite( LED3, HIGH);
}


void loop()
{
///////////////1st relay///////////////////
{
if (digitalRead(Button)==HIGH) //if button is pressed
{
if ( LED_state == LOW)
{//if the LED is off at this moment,
digitalWrite( LED, HIGH); //turn it on
LED_state = HIGH; //new status of the LED is HIGH
}
else
{//if the LED is on at this moment,
digitalWrite( LED, LOW); //trun it off
LED_state = LOW; //new status of the LED is LOW 
} 
} 
//delay(300); //A very simple way to handle button bouncing, but need to adjust the delay time to optimize the result.
}
/////////2nd relay///////////////
{
if (digitalRead(Button2)==HIGH) //if button is pressed
{
if ( LED_state2 == LOW)
{//if the LED is off at this moment,
digitalWrite( LED2, HIGH); //turn it on
LED_state2 = HIGH; //new status of the LED is HIGH
}
else
{//if the LED is on at this moment,
digitalWrite( LED2, LOW); //trun it off
LED_state2 = LOW; //new status of the LED is LOW 
} 
} 
//delay(300); //A very simple way to handle button bouncing, but need to adjust the delay time to optimize the result.
}
////////3 relay///////////////////
{
if (digitalRead(Button3)==HIGH) //if button is pressed
{
if ( LED_state3 == LOW)
{//if the LED is off at this moment,
digitalWrite( LED3, HIGH); //turn it on
LED_state3 = HIGH; //new status of the LED is HIGH
}
else
{//if the LED is on at this moment,
digitalWrite( LED3, LOW); //trun it off
LED_state3 = LOW; //new status of the LED is LOW 
} 
} 
//delay(300); //A very simple way to handle button bouncing, but need to adjust the delay time to optimize the result.
}
}
