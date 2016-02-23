
// this is button_watcher, a test for button bounce --- free and open code Oct 25, 2014 by GFS
// this can be used to check buttons for needed debounce time.
// if you get multiple UP and DOWN messages for one press or release, increase debounce.

byte ledPin = 7;
byte buttonPin = 31; // my button is a jumper wire grounded on the metal USB port box.

byte pinStates; // always the current state is in bit 0 and the previous is in bit 1
// value --- meaning with pin mode INPUT_PULLUP.
// 0 --- no change, pressed now and pressed last check
// 1 --- changed, released now, pressed last check
// 2 --- changes, pressed now, released last check
// 3 --- no change, released now and released last check

byte buttonState; // this only changes when the pin states are stable for some time.
byte buttonPrev; // this only changes when the pin states are stable for some time.

unsigned long checkCounter; // how many checks since last stable change


void setup( void )
{
  Serial.begin( 115200 );
  Serial.println( "\n  Button Bounce Check" );

  pinMode( ledPin, OUTPUT );  // default os LOW
  pinMode( buttonPin, INPUT_PULLUP );  // default is pullup HIGH
  pinStates = 1; // sets initial pin state as not pressed

  // buttonState logic will be 0=false for stable not pressed, 1=true for stable pressed
}

unsigned long btNow, btStart, btDebounce = 2000UL; // bt = button time. 2ms debounce

inline void buttonHandler( void ) // just so the cleanliness police don't freak out
{
  pinStates &= 1; // mask off the old previous state bit, only bit 0 data is left
  pinStates <<= 1; // move current pin state bit left 1 into previous pin state bit

  pinStates += digitalRead( buttonPin ); // read the current pin state into bit 0

  btNow = micros(); // time at loop start and read pin state to within 4 usecs

    switch ( pinStates )  // IDE autoformat indents this line. Why?
  {
  case  0 :  // 0 --- no change, pressed now and pressed last check
    if ( btNow - btStart >= btDebounce )
    {
      buttonState = 1;
      btStart = btNow;  
    }
    break;

  case  1 :  // 1 --- changed, released now, pressed last check
    Serial.print( '_' ); // show the bounce 
    Serial.println( btNow - btStart ); // show the micros since last stable or change
    btStart = btNow;  
    break;

  case  2 :  // 2 --- changes, pressed now, released last check
    Serial.print( '*' ); // show the bounce
    Serial.println( btNow - btStart ); // show the micros since last stable or change
    btStart = btNow; 
    break;

  case  3 :  // 3 --- no change, released now and released last check
    if ( btNow - btStart >= btDebounce )
    {
      buttonState = 0;
      btStart = btNow;  
    }
    break;
  }
}

void loop( void )
{
  buttonHandler( );
  checkCounter++;

  if ( buttonState != buttonPrev ) // button stable state change event
  {
    buttonPrev = buttonState;
    digitalWrite( ledPin, buttonState );
    Serial.print( F( "\nButton is " ));
    if ( buttonState ) // if not zero, it is true
    {
      Serial.print( F( " DOWN " ));
    }
    else
    {
      Serial.print( F( " UP " ));
    }
    Serial.print( F( "after " ));
    Serial.print( checkCounter );
    Serial.println( F( " checks.\n" ));
    checkCounter = 0UL;
  }
}
