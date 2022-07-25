/*
                      THIS VERSION IS FINAL, 2018 REVISION THAT
                      SIGNIFICANTLY IMPROVES THE USER EXPERIENCE
                      OVER THE 2017 VERSION. THIS VERSION IS FASTER,
                      SLEEKER, CLEANER, EASIER TO PICK UP AND PLAY

                      THIS VERSION REPLACES ALL OTHER VERSIONS

    Hallowe'en 2017
    REVISED OCTOBER 29, 2018

    Fallout 3-inspired Terminal to hack and act as a game the kids can play to
    upgrade their candy.

    Enable caps lock on connected keyboard.

    Special controls: '!' to reset input in event of typo
                      ':' to call instruction set if needed

    The monitor for this is a circa 1980s analog TV, connected to the PC via HDMI-to-RCA
    converter box. The text is displayed over CoolTerm, the free terminal emulation software.

    The settings in CoolTerm are very important for the functionality of this sketch over
    CoolTerm, where a kid who sort of just mashes on the keys will cause the terminal emulator
    to freak out, ruining the mystique of the prop, as well as the ability to understand and
    operate it. This problem was not present while running it over the Arduino Serial Monior,
    so I changed the settings in CoolTerm to get it to work nicely. I was using a 7-inch display,
    so the font size itself reflects that, and the need for the text to be large enough to read.
    The settings for CoolTerm are:

               under Preferences:
                                  Text Color (custom, lime greenish)
                                  Background color, black as possible
                                  uncheck Condense Line Spacings
                                  Font: Fixedsys, size 36

               under  Options--Serial Port:

                                  Baudrate: 9600, 8N1, no Flow Control

    Password functionality derived from Password library example sketch:"Serial Monitor"
    Unlike the example sketch, this auto resets the entries upon exiting either any of the
    failed entry functions (terminalLocked() and wrongAnswer()), or the correct entry function
    (rightAnswer()).

    Servo function is as prototyped in sketch fallout3ServoPrototypeandTuning.

    Servo uses pin 10.

    Buttons on pins 2 through 8 call the debouncer class to create a simple interface for the kids
    to enter or password and evaluate their guesses.

    This is for the Uno, Mega, etc. Due Version separate, and may allow standalone
    USB keyboard entry.

    Written by Hallowed31
    Testing and Debugging by Coltonius31
    September 2017

    REVISED 2018
    Revisions: Mainly detached the LEDs from the buttons to allow them to run an
              entertaining script instead. Also cleaned up the finicky buttons because
              they were constantly missing the button presses, which ruined the game.
              Now, all the buttons simply do the same thing: evaluate the password.
              Also edited instruction set due to button fix.
              Also added an automatic reset of keyboard entry when keyboard presses
              exceed the length of the longest word in the list ("costume"). Should
              cue the user not to simply button mash and crash the program. Also put
              all LEDs on flashy blinky script. They just look cool, no funtionality
              to the program
*/

#include <Password.h>
#include <Servo.h>

Password password = Password("CANDY");
Servo myservo;  // create servo object

byte currentLength = 0;  // variable to store the length of keyboard input at any given time

byte numberChances = 4;
byte thisAttempt = 4;
byte lastAttempt = 0; //this is the one that sends sketch to terminalLocked()

int pos = 0;    // variable to store the servo position


void crackSafe() {

  myservo.attach(10);  // attaches the servo on pin 10 to the servo object

  /* pop the microwave oven door */
  for (pos = 0; pos <= 180; pos += 10) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
  for (pos = 180; pos >= 0; pos -= 10) { // goes from 180 degrees to 0 degrees
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }

  /* lets open it twice, as the microwave oven latch can be inconsistent */
  for (pos = 0; pos <= 180; pos += 10) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
  for (pos = 180; pos >= 0; pos -= 10) { // goes from 180 degrees to 0 degrees
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
  myservo.detach(); //cleanup before we leave, prevents servo jitter
}

/* kid wins entry to the safe, to take one beverage (Coca Cola or juice box) */
void rightAnswer()
{
  //add some space to the screen
  for (int h = 0; h < 4; h++) {
    Serial.println();
  }

  Serial.print(F("          EXACT MATCH!"));
  for (int h = 0; h < 4; h++) {
    Serial.println();
  }
  delay(500);

  Serial.print(F(">>>>>>>>  ACCESS GRANTED "));
  delay(2000);
  for (int x = 0; x < 2; x++) {
    Serial.println();
  }
  delay(1000);
  quickScroll();
  crackSafe();

  Serial.print(F("      WELCOME TRICK OR TREATER"));
  delay(3000); // added more time for kids to notice and read previous line
  for (int x = 0; x < 2; x++) {
    Serial.println();
  }

  Serial.print(F("        COMIC BOOK (1)   <TAKE>"));
  //hold the message for a bit...
  delay(6000);
  //clear the screen for the next person
  for (int i = 0; i < 10; i++) {
    Serial.println();
    delay(200);
  }
  /* clean up when before we leave, reset attempts */
  numberChances = 4;
  thisAttempt = 4;
  delay(3000);
  //reset for the nest trick-or-treater
  //candyState = LOW;
  robcoResetProtocol();
}

class Flasher
{
    // Class Member Variables
    // These are initialized at startup
    int ledPin;      // the number of the LED pin
    long OnTime;     // milliseconds of on-time
    long OffTime;    // milliseconds of off-time

    // These maintain the current state
    int ledState;                 // ledState used to set the LED
    unsigned long previousMillis;   // will store last time LED was updated

    // Constructor - creates a Flasher
    // and initializes the member variables and state
  public:
    Flasher(int pin, long on, long off)
    {
      ledPin = pin;
      pinMode(ledPin, OUTPUT);

      OnTime = on;
      OffTime = off;

      ledState = LOW;
      previousMillis = 0;
    }

    void Update()
    {
      // check to see if it's time to change the state of the LED
      unsigned long currentMillis = millis();

      if ((ledState == HIGH) && (currentMillis - previousMillis >= OnTime))
      {
        ledState = LOW;  // Turn it off
        previousMillis = currentMillis;  // Remember the time
        digitalWrite(ledPin, ledState);  // Update the actual LED
      }
      else if ((ledState == LOW) && (currentMillis - previousMillis >= OffTime))
      {
        ledState = HIGH;  // turn it on
        previousMillis = currentMillis;   // Remember the time
        digitalWrite(ledPin, ledState);   // Update the actual LED
      }
    }
};

class debouncer
{
    int buttonPin; //button to enter response, ANY BUTTON (2018 revision)
    boolean currentButton;
    boolean lastButton;

  public:
    debouncer(int button, boolean was, boolean is)
    {
      buttonPin = button;
      pinMode(buttonPin, INPUT_PULLUP); //pin is HIGH when button is not pressed, so logic is inverted

      lastButton = was;
      lastButton = HIGH;

      currentButton = is;
      currentButton = HIGH;
    }

    boolean debounce(boolean was)
    {
      boolean is = digitalRead(buttonPin);
      if (was != is)
      {
        delay(10);
        is = digitalRead(buttonPin);
      }
      return is;
    }

    void Update()
    {
      currentButton = debounce(lastButton);

      if ((lastButton == LOW) && (currentButton == HIGH)) { //evaluate password HERE
        /*    FIRST THE NULL OR WRONG BUTTON PRESSES...   */

        /* button is reverse logic due to setup assignment of INPUT_PULLUP */
        //OK, so you got it right by either the password or the correct button
        if ((password.evaluate()) && (numberChances > 0)) {
          rightAnswer();
        }

        /* if you have no chances left (numberChances), straight to terminalLocked()
          with no prettyprinting */
        else if (numberChances == 0) {
          terminalLocked();
        }

        /* first answer was wrong, still have 3 chances remaining */
        else if ((!password.evaluate()) && (numberChances > lastAttempt) && (numberChances == 4)) {

          numberChances--;
          firstWrongAnswer();
        }

        /* second answer was wrong, still have 2 chances remaining */
        else if ((!password.evaluate()) && (numberChances > lastAttempt) && (numberChances == 3)) {

          numberChances--;
          secondWrongAnswer();
        }

        /* third answer was wrong, still have 1 chance remaining */
        else if ((!password.evaluate()) && (numberChances > lastAttempt) && (numberChances == 2)) {

          numberChances--;
          lockoutImminent();
          //thirdWrongAnswer() gives the easyPeasy game list
          thirdWrongAnswer();
        }

        /* last chance! */
        else if ((!password.evaluate()) && (numberChances > lastAttempt) && (numberChances == 1)) {
          terminalLocked();
          numberChances--;
        }

        /* if numberChances is zero, straight to locked out */
        else if ((password.evaluate()) && (numberChances == 0)) {
          terminalLocked();
        }

        /* if you tried to enter a response when already at zero chances... */
        else if ((!password.evaluate()) && (numberChances == 0)) {
          terminalLocked();
        }

        lastButton = currentButton;
      }

      else if ((lastButton == HIGH) && (currentButton == LOW)) {
        lastButton = currentButton;
      }
    }
};

Flasher led1(12, 100, 400);
Flasher led2(14, 350, 350);
Flasher led3(15, 100, 400);
Flasher led4(16, 350, 350);
Flasher led5(17, 100, 400);
Flasher led6(18, 350, 350);


// Three arguments (buttonPin, was, is): 2018 revision is that all buttons simply evaluate password.
debouncer button2 (2, 1, 1);
debouncer button3 (3, 1, 1);
debouncer button4 (4, 1, 1);
debouncer button5 (5, 1, 1);
debouncer button6 (6, 1, 1);
debouncer button7 (7, 1, 1);
debouncer button1 (8, 1, 1);

void setup() {
  Serial.begin(9600); // 2018 rev: Coolterm defaults to 9600 upon opening, so takes out a step and guesswork
  delay(1000);
  /* to do away with the leftover text from last time thhis was plugged in... */
  for (int t = 0; t < 22; t++) {  // 2018 revision, default 25
    Serial.println();
  }
  robcoResetProtocol();
}


void loop() {

  led1.Update();
  led2.Update();
  led3.Update();
  led4.Update();
  led5.Update();
  led6.Update();

  button1.Update();
  button2.Update();
  button3.Update();
  button4.Update();
  button5.Update();
  button6.Update();
  button7.Update();

  if (Serial.available()) {
    char input = Serial.read();

    switch (input) {
      case '!': //reset password
        password.reset();
        currentLength = 0;
        Serial.println("      >PASSWORD RESET<");
        Serial.println();
        Serial.println("           ENTER PASSWORD");
        break;
      case '?': //evaluate password
        break;
      case ':': //run robcoResetProtocol in case we need the instructions
        password.reset();
        currentLength = 0;
        robcoResetProtocol();
        break;

      default: //append any keypress that is not a '!' nor a '?' to the currently guessed password.
        password << input;
        currentLength++;
        if (currentLength > 7) {
          password.reset(); // to avoid button mashing
          currentLength = 0;
          Serial.println("            >SYNTAX ERROR<");
          Serial.println();
          Serial.println("         >INPUT EXCEEDS RX BUFFER<");
          Serial.println();
          Serial.println("        >SELECT PASSWORD FROM LIST<");
          Serial.println();
          delay(2500);
          //wait for all the garbage
          discardIncomingSerial();
          quickScroll();
          Serial.println("           >PASSWORD RESET<");
          Serial.println();
          Serial.println("           ENTER PASSWORD");
          delay(2000);
          buttonMashProtocol(); // reprint the word list
          break; // now get outta here and try again
        }
        //the asterisks for textual display that some text is being typed
        for (byte i = 0; i < currentLength; i++) {
          Serial.print('*');
        }
        Serial.println();
    }
  }
}

/*This occurs after a successful hacked teminal, or after one
  that has been locked out, or at the start of program  */
void robcoResetProtocol() {

  /* eight-second delay here so next kid in line has chance to step up */
  delay(2000); // default 8000 for actual thing
  Serial.println(); //for prettyprinting
  Serial.println();
  Serial.println(F("  WELCOME TO ROBCO INDUSTRIES(TM) [TERMLINK]"));
  Serial.println();
  delay(1000);

  Serial.println(F("  >SET TERMINAL/INQUIRE"));
  Serial.println();
  delay(1000);

  Serial.println(F("  RIT-V300"));
  Serial.println();
  delay(1000);

  Serial.println(F("  >SET FILE/PROTECTION=OWNER:RWED /ACCOUNTS.F"));
  delay(800);
  Serial.println(F("  >SET HALT RESTART/MAINT"));
  Serial.println();
  delay(1200);

  Serial.println(F("  Initializing Robco Ind(TM) MF Boot :Agent v2.3.0"));
  delay(800);
  Serial.println(F("  RETROS BIOS"));
  delay(800);
  Serial.println(F("  RBIOS-4.02.08.00 52EE5.E7.E8"));
  delay(800);
  Serial.println(F("  Copyright 2201-2203 Robco Ind."));
  delay(800);
  Serial.println(F("  Uppermem: 64KB"));
  delay(800);
  Serial.println(F("  Root (5A8)"));
  delay(800);
  Serial.println(F("  Maintenance Mode"));
  Serial.println();
  delay(1500);

  Serial.println(F("  >RUN DEBUG/ACCOUNTS.F"));
  Serial.println();
  Serial.println();
  Serial.println();
  Serial.println();
  delay(1000);

  for (int i = 0; i < 15; i++) {
    Serial.println();
  }

  Serial.println(F("  ROBCO INDUSTRIES (TM) TERMLINK :PROTOCOL"));
  delay(1000);

  //make room for the instructions so not overlooked
  scrollUp();
  Serial.println(F("        4 ATTEMPTS AT PASSWORD"));
  delay(2000);

  scrollUpFewer();
  Serial.println(F("           KEY IN PASSWORD"));
  Serial.println();
  delay(2000);         //enough time to read the text //DEFAULT 7000
  Serial.println();
  Serial.println(F("    "));
  Serial.println(F(" PRESS ANY BUTTON TO CHECK PASSWORD"));
  delay(4000);        //more time to comprehend the instructions DEFALUT 8000

  quickScroll();

  Serial.println(F(" 0xF9AC [$_:|>/>]<  0xFA78 %GHOUL</=?"));
  Serial.println(F(" 0xF9B8 [$_:|>/>GH  0xFA84 ?_^#!</=FA"));
  Serial.println(F(" 0xF9C4 OST:|>/>?=  0xFA90 NGS#!<:/=?"));
  Serial.println(F(" 0xF9E8 #^_:|>/>]<  0xFA04 ?_^#CANDY?"));
  Serial.println(F(" 0xFA30 [$_TREAT]<  0xFB20 ?_^#!<:/=?"));
  Serial.println(F(" 0xFA54 [$_:|>/>]<  0xFAA8 ?_^=TRICK?"));
  Serial.println(F(" 0xF9AC [$_:|>COST  0xFA9C ?_GHOUL/=?"));
  Serial.println(F(" 0xF9AC UME:|>/>]<  0xFB04 ?_^#!<:/=?"));
  Serial.println(F(" 0xF9E8 #^_:|>/>]<  0xFA04 ?_^#CANDY?"));

  password.reset();
  currentLength = 0;
}

/* For subsequent attempts after wrong answer */
void robco3Protocol()
{
  Serial.println(F("ROBCO INDUSTRIES (TM) TERMLINK PROTOCOL"));
  delay(500);
  Serial.println();
  Serial.println(F("        3 ATTEMPT(S) REMAINING"));
  Serial.println();
  delay(500);
  Serial.println(F("     ANY BUTTON TO ENTER PASSWORD "));
  Serial.println();
  delay(1000);
  Serial.println(F("          ENTER PASSWORD: "));
  delay(1000);
  Serial.println(F(" 0xF9AC [$_:|>/>]<  0xFA78 %FANGS</=?"));
  Serial.println(F(" 0xF9B8 [$_:|>/>GH  0xFA84 ?_^#!<:/=?"));
  Serial.println(F(" 0xF9C4 OST:|>/>]<  0xFA90 ?_^#!<:/=?"));
  Serial.println(F(" 0xF9E8 [$_:|>/>]<  0xFA04 ?_^#TRICK?"));
  Serial.println(F(" 0xFA30 [$_TREAT]<  0xFB20 ?_^#!<:/=?"));
  Serial.println(F(" 0xFA54 [$_:|>/>]<  0xFAA8 ?_^BONES=?"));
  Serial.println(F(" 0xF9AC [$_:|>COST  0xFA9C ?_^#!<:/=?"));
  Serial.println(F(" 0xF9AC UME:|>/>]<  0xFB04 ?_^#CANDY?"));
  Serial.println(F(" 0xF9E8 [$_:|>/>]<  0xFA04 ?_^#TRICK?"));

  password.reset();
  currentLength = 0;
}

void robco2Protocol()
{
  Serial.println(F("ROBCO INDUSTRIES (TM) TERMLINK PROTOCOL"));
  delay(500);
  Serial.println();
  Serial.println(F("        2 ATTEMPT(S) REMAINING"));
  delay(500);
  Serial.println();
  Serial.println(F("       ANY BUTTON TO ENTER PASSWORD "));
  Serial.println();
  delay(1000);
  Serial.println(F("          ENTER PASSWORD: "));
  delay(1000);
  Serial.println(F(" 0xF9AC [$_:|>/>]<  0xFA78 %FANGS</=?"));
  Serial.println(F(" 0xF9B8 [$_:|>/>GH  0xFA84 ?_^#!<:/=?"));
  Serial.println(F(" 0xF9C4 OST:|>/>]<  0xFA90 ?_^#!<:/=?"));
  Serial.println(F(" 0xF9E8 [$_:|>/>]<  0xFA04 ?_^#TRICK?"));
  Serial.println(F(" 0xFA30 [$_TREAT]<  0xFB20 ?_^#!<:/=?"));
  Serial.println(F(" 0xFA54 [$_:|>/>]<  0xFAA8 ?_^BONES=?"));
  Serial.println(F(" 0xF9AC [$_:|>COST  0xFA9C ?_^#!<:/=?"));
  Serial.println(F(" 0xF9AC UME:|>/>]<  0xFB04 ?_^#CANDY?"));
  Serial.println(F(" 0xFA30 [$_TREAT]<  0xFB20 ?_^#!<:/=?"));

  password.reset();
  currentLength = 0;
}


/* intended to be the robcoProtocol on last chance, so words almost all CANDY
    to make it really easy, if the kids are payin' attention...
*/
void robcoProtocolEasyPeasy() {
  Serial.println(F("ROBCO INDUSTRIES (TM) TERMLINK PROTOCOL"));
  delay(500);
  Serial.println();
  Serial.println();
  Serial.println(F("         1 ATTEMPT REMAINING"));
  Serial.println();
  Serial.println(F("       ANY BUTTON TO ENTER PASSWORD "));
  Serial.println();
  delay(1000);
  Serial.println(F("          ENTER PASSWORD: "));
  delay(1000);
  Serial.println(F(" 0xF9AC [$_:|>/>]<  0xFA78 %CDNAY</=?"));
  Serial.println(F(" 0xF9B8 [$_:|>/>CA  0xFA84 ?_^#!<:/=?"));
  Serial.println(F(" 0xF9C4 NDY:|>/>]<  0xFA90 ?_^#!<:/=?"));
  Serial.println(F(" 0xF9E8 [$_:|>/>]<  0xFA04 ?_CANDY#@?"));
  Serial.println(F(" 0xFA30 [$_CANDY]<  0xFB20 ?_^#!<:/=?"));
  Serial.println(F(" 0xFA54 [$_:|>/>]<  0xFAA8 ?_^CNADY=?"));
  Serial.println(F(" 0xF9AC [$_:|>>^CA  0xFA9C ?_^#!<:/=?"));
  Serial.println(F(" 0xF9AC NDY:|>/>]<  0xFB04 ?_^#CANDY?"));
  Serial.println(F(" 0xF9AC TPYE|>/>]<  0xFB04 ?_^#CANDY?"));

  password.reset();
  currentLength = 0;
}


void firstWrongAnswer()
{
  //make room for the message
  for (int n = 0; n < 6; n++) {
    Serial.println();
  }
  Serial.println();
  Serial.println(F("             ACCESS DENIED"));
  delay(2000);
  Serial.println();
  //scroll up the screen quicker than rightAnswer or terminalLocked
  for (int n = 0; n < 10; n++) {
    Serial.println();
    delay(100);
  }
  robco3Protocol();
}

void secondWrongAnswer()
{
  //make room for the message
  for (int n = 0; n < 6; n++) {
    Serial.println();
  }
  Serial.println();
  Serial.println(F("             ACCESS DENIED"));
  delay(2000);
  Serial.println();
  //scroll up the screen quicker than rightAnswer or terminalLocked
  for (int n = 0; n < 10; n++) {
    Serial.println();
    delay(100);
  }
  robco2Protocol();
}

void thirdWrongAnswer()
{
  //make room for the message
  for (int n = 0; n < 6; n++) {
    Serial.println();
  }
  Serial.println();
  Serial.println(F("             ACCESS DENIED"));
  delay(2000);
  Serial.println();
  //scroll up the screen quicker than rightAnswer or terminalLocked
  for (int n = 0; n < 10; n++) {
    Serial.println();
    delay(100);
  }
  robcoProtocolEasyPeasy();
}

/* last chance before being locked out, ie third wrong answer here's a warning... */
void lockoutImminent() {
  //make room for the message
  for (int n = 0; n < 6; n++) {
    Serial.println();
  }
  Serial.println(F("      WARNING: LOCKOUT IMMINENT!!!!"));
  Serial.println();
  Serial.println();
  delay(1500);
  Serial.println(F("      WARNING: LOCKOUT IMMINENT!!!!"));
  delay(500);
  Serial.println();
  Serial.println();
  //scroll up the screen quicker than rightAnswer or terminalLocked
  for (int n = 0; n < 10; n++) {
    Serial.println();
    delay(300);
  }
}

/* kid still gets consolation candy, I mean I'm not an ogre */
void terminalLocked() {

  //Clear the screen for the locked message
  for (int i = 0; i < 15; i++) {
    Serial.println();
  }

  Serial.println(F("           Terminal Locked "));
  Serial.println();
  Serial.println(F("       Check With Administrator "));
  Serial.println(); // 3 BLANK LINES TO VERTICALLY CENTER THE TEXT
  Serial.println();
  Serial.println();
  /* clean up when before we leave, reset attempts */
  numberChances = 4;
  thisAttempt = 4;
  delay(10000);

  //clear the screen for the next person
  //scroll up the screen
  for (int t = 0; t < 10; t++) {
    Serial.println();
    delay(200);
  }
  //reset for the nest trick-or-treater
  robcoResetProtocol();
}

void scrollUp() {
  for (int n = 0; n < 9; n++) {
    Serial.println();
    delay(300); //makes it look legit
  }
}

void scrollUpFewer() {
  for (int n = 0; n < 4; n++) {
    Serial.println();
    delay(300); //makes it look legit
  }
}

void quickScroll() {
  for (int q = 0; q < 9; q++) {
    Serial.println();
    delay(30);
  }
}

void discardIncomingSerial() {
  while (Serial.available())
    Serial.read();
}

void buttonMashProtocol() {
  Serial.println(F(" 0xF9AC [$_:|>/>]<  0xFA78 %FANGS</=?"));
  Serial.println(F(" 0xF9B8 [$_:|>/>GH  0xFA84 ?_^#!<:/=?"));
  Serial.println(F(" 0xF9C4 OST:|>/>]<  0xFA90 ?_^#!<:/=?"));
  Serial.println(F(" 0xF9E8 [$_:|>/>]<  0xFA04 ?_^#TRICK?"));
  Serial.println(F(" 0xFA30 [$_TREAT]<  0xFB20 ?_^#!<:/=?"));
  Serial.println(F(" 0xFA54 [$_:|>/>]<  0xFAA8 ?_^BONES=?"));
  Serial.println(F(" 0xF9AC [$_:|>COST  0xFA9C ?_^#!<:/=?"));
  Serial.println(F(" 0xF9AC UME:|>/>]<  0xFB04 ?_^#CANDY?"));
  Serial.println(F(" 0xFA30 [$_TREAT]<  0xFB20 ?_^#!<:/=?"));
}
