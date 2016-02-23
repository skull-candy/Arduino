int interrupt_pin = 2;
int led_pin = 2;

volatile int toggle = 0;

/***************************************************
 *  Name:        pin2Interrupt
 *
 *  Returns:     Nothing.
 *
 *  Parameters:  None.
 *
 *  Description: Service routine for pin2 interrupt  
 *
 ***************************************************/
void pin2Interrupt(void)
{
  /* This will bring us back from sleep. */
  toggle = 1;
  
  /* Delay in here as a very crude debouncing mechanism. */ 
  delay(100);
}



/***************************************************
 *  Name:        setup
 *
 *  Returns:     Nothing.
 *
 *  Parameters:  None.
 *
 *  Description: Setup for the Arduino.           
 *
 ***************************************************/
void setup()
{
  Serial.begin(9600);
  
  /* Setup the interrupt pin */
  pinMode(interrupt_pin, INPUT);
  attachInterrupt(0, pin2Interrupt, FALLING);
  
  Serial.println("Initialisation complete.");
}



/***************************************************
 *  Name:        loop
 *
 *  Returns:     Nothing.
 *
 *  Parameters:  None.
 *
 *  Description: Main application loop.
 *
 ***************************************************/
int seconds=0;
void loop()
{
  if (toggle == 1)
  {
    Serial.println("Toggle");
    digitalWrite(led_pin, !digitalRead(led_pin));
    toggle = 0;
  }
}
