#include <LiquidCrystal.h>

//By Stefan Chochowski

// initialize the library with the numbers of the interface pins

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

const int button1 = 7;

const int button2 = 8;

const int button3 = 9;

const int button4 = 10;



int button1state = 0;

int button2state = 0;

int button3state = 0;

int button4state = 0;



int check = 0;



int unlock_var = 0;

int unlock_code = 100;

int temp_num;

int gate = 0;



void setup() {



  pinMode(button1, INPUT);

  pinMode(button2, INPUT);

  pinMode(button3, INPUT);

  pinMode(button4, INPUT);

  // set up the LCD's number of columns and rows:

  lcd.begin(16, 2);

  // Print a message to the LCD.

  lcd.print("System State:");

}





void loop() {

  // read the state of the pushbutton value:

  button1state = digitalRead(button1);

  button2state = digitalRead(button2);

  button3state = digitalRead(button3);

  button4state = digitalRead(button4);



  //  lcd.print("                   ");

  //  // check if the pushbutton is pressed.

  //  // if it is, the buttonState is HIGH:

  //  if (button1state == HIGH) {

  //    // show "yes" message

  //    lcd.setCursor(0,1);

  //    lcd.print("Pressed");

  //  }

  //  else {

  //    // show "no" message

  //    lcd.setCursor(0,1);

  //    lcd.print("NOT Pressed");

  //  }







  lcd.setCursor(0, 1);

  lcd.print("Waiting...        ");



  if (button1state == HIGH)

  {

    unlock_var = 26;

    lcd.setCursor(0, 1);

    lcd.print("Reading...");

    delay(500);

  }

  if (button2state == HIGH)

  {

    unlock_var = unlock_var * 3;

    lcd.setCursor(0, 1);

    lcd.print("Reading...");

    delay(500);

  }

  if (button3state == HIGH)

  {

    temp_num = unlock_var;

    temp_num = temp_num / 2;

    temp_num = temp_num - 17;

    unlock_var = unlock_var + temp_num;

    lcd.setCursor(0, 1);

    lcd.print("Reading...");

    delay(500);

  }



  if (button4state == HIGH)

  {

    check = 1;

  }



  if (check == 1)

  {

    lcd.setCursor(0, 1);

    lcd.print("Checking Code...");

    delay(1000);



    if (unlock_var == unlock_code)

    {

      lcd.setCursor(0, 1);

      lcd.print("Access Granted    ");

      delay(10000); //unlock delay

      unlock_var = 0;

    }

    else

    {

      lcd.setCursor(0, 1);

      lcd.print("Access Denied     ");

      delay(1500);

      unlock_var = 0;

      check = 0;

    }



  }

}


