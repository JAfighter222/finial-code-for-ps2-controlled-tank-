//#include <Adafruit_NeoPixel.h>




#include <Servo.h>
#include <L298N.h>
//#ifdef __AVR__
//#include <avr/power.h>
//#endif
//#define PIN        39
//#define NUMPIXELS 6 // Popular NeoPixel ring size
//Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
//#define DELAYVAL 500 // Time (in milliseconds) to pause between pixels

#define ENa 2
#define IN1 3
#define IN2 8
#define ENB 5
#define IN3 7
#define IN4 6
L298N motor1(ENa, IN1, IN2);
L298N motor2(ENB, IN3, IN4);

Servo myservo;
Servo myservo2;
int verA = 0;//vertical angle of servo
int horA = 0;// horsintial angle of the servo

#include <PS2X_lib.h>  //for v1.6

PS2X ps2x; // create PS2 Controller Class

//right now, the library does NOT support hot pluggable controllers, meaning
//you must always either restart your Arduino after you conect the controller,
//or call config_gamepad(pins) again after connecting the controller.
int error = 0;
byte type = 0;
byte vibrate = 0;



void setup() {
  pinMode(53, OUTPUT);
  //#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  //clock_prescale_set(clock_div_1);
  //#endif
  //pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)

  myservo2.attach(10);
  myservo.attach(9);
  myservo2.write(0);
  myservo.write(0);

  Serial.begin(57600);

  motor1.setSpeed(255); // an integer between 0 and 255
  motor2.setSpeed(255); // an integer between 0 and 25
  //CHANGES for v1.6 HERE!!! **************PAY ATTENTION*************

  error = ps2x.config_gamepad(13, 11, 4, 12, true, true); //setup pins and settings:  GamePad(clock, command, attention, data, Pressures?, Rumble?) check for error

  if (error == 0) {
    Serial.println("Found Controller, configured successful");
    Serial.println("Try out all the buttons, X will vibrate the controller, faster as you press harder;");
    Serial.println("holding L1 or R1 will print out the analog stick values.");
    Serial.println("Go to www.billporter.info for updates and to report bugs.");
  }

  else if (error == 1)
    Serial.println("No controller found, check wiring, see readme.txt to enable debug. visit www.billporter.info for troubleshooting tips");

  else if (error == 2)
    Serial.println("Controller found but not accepting commands. see readme.txt to enable debug. Visit www.billporter.info for troubleshooting tips");

  else if (error == 3)
    Serial.println("Controller refusing to enter Pressures mode, may not support it. ");

  //Serial.print(ps2x.Analog(1), HEX);

  type = ps2x.readType();
  switch (type) {
    case 0:
      Serial.println("Unknown Controller type");
      break;
    case 1:
      Serial.println("DualShock Controller Found");
      break;
    case 2:
      Serial.println("GuitarHero Controller Found");
      break;
  }

}

void loop() {
  //pixels.clear(); // Set all pixel colors to 'off'

  // The first NeoPixel in a strand is #0, second is 1, all the way up
  // to the count of pixels minus one.
  //for(int i=0; i<NUMPIXELS; i++) { // For each pixel...

  // pixels.Color() takes RGB values, from 0,0,0 up to 255,255,255
  // Here we're using a moderately bright green color:
  // pixels.setPixelColor(i, pixels.Color(0, 0, 0));

  //pixels.show();   // Send the updated pixel colors to the hardware.

  //delay(DELAYVAL); // Pause before next pass through loop
  //}
  /* You must Read Gamepad to get new values
    Read GamePad and set vibration values
    ps2x.read_gamepad(small motor on/off, larger motor strenght from 0-255)
    if you don't enable the rumble, use ps2x.read_gamepad(); with no values

    you should call this at least once a second
  */



  if (error == 1) //skip loop if no controller found
    return;

  if (type == 2) { //Guitar Hero Controller

  }
  else { //DualShock Controller

    ps2x.read_gamepad(false, vibrate);          //read controller and set large motor to spin at 'vibrate' speed

    if (ps2x.Button(PSB_START)) {                 //will be TRUE as long as button is pressed
      Serial.println("Start is being held");
    }
    if (ps2x.Button(PSB_SELECT)) {
      Serial.println("Select is being held");

    }
    if (ps2x.ButtonPressed(PSB_PAD_UP)) {        //will be TRUE as long as button is pressed
      Serial.print("Up presed: ");
      Serial.println(ps2x.Analog(PSAB_PAD_UP), DEC);

      if (verA < 180) {
        verA += 10;
      }
      //Serial.println(verA);
      myservo.write(verA);
    }




    if (ps2x.ButtonPressed(PSB_PAD_DOWN)) {
      Serial.print("DOWN held this hard: ");
      Serial.println(ps2x.Analog(PSAB_PAD_DOWN), DEC);
      if (verA > 0) {
        verA -= 10;
      }
      //Serial.println(verA);
      myservo.write(verA);


    }
    //delay(2000);

    if (ps2x.ButtonPressed(PSB_PAD_RIGHT)) {
      Serial.println("Right held this hard: ");
      Serial.println(ps2x.Analog(PSAB_PAD_RIGHT), DEC);
      if (horA > 0) {
        horA -= 10;
      }
      //   Serial.println(horA);
      myservo2.write(horA);

    }
    if (ps2x.ButtonPressed(PSB_PAD_LEFT)) {
      Serial.println("LEFT held this hard: ");
      Serial.println(ps2x.Analog(PSAB_PAD_LEFT), DEC);
      if (horA < 180) {
        horA += 10;
      }
      //  Serial.println(verA);
      myservo2.write(horA);
    }



    // vibrate = ps2x.Analog(PSAB_BLUE);        //this will set the large motor vibrate speed based on
    //how hard you press the blue (X) button

    if (ps2x.NewButtonState())               //will be TRUE if any button changes state (on to off, or off to on)
    {



      if (ps2x.Button(PSB_L3)) {
        Serial.println("L3 pressed");
      }
      if (ps2x.Button(PSB_R3)) {
        Serial.println("R3 pressed");
      }

      if (ps2x.ButtonPressed(PSB_L2)) {
        Serial.println("L2 pressed");
        motor2.forward();
        Serial.println("motor go");
      }
      if (ps2x.ButtonPressed(PSB_R2 )) {
        Serial.println("R2 pressed");
        motor1.forward();
        Serial.println("motor go");
      }
      if (ps2x.ButtonReleased(PSB_R2 )) {
        motor1.stop();
        Serial.println("motor stop");
      }
      if (ps2x.ButtonReleased(PSB_L2 )) {
        motor2.stop();
        Serial.println("motor stop");
      }
      if (ps2x.ButtonPressed(PSB_L1)) {
        motor2.backward();
        Serial.println("motor go");
      }
      if (ps2x.ButtonPressed(PSB_R1)) {
        motor1.backward();
        Serial.println("motor go");
      }
      if (ps2x.ButtonReleased(PSB_L1)) {
        motor2.stop();
        Serial.println("motor stop");
      }
      if (ps2x.ButtonReleased(PSB_R1)) {
        motor1.stop();
        Serial.println("motor stop");
      }


      if (ps2x.ButtonReleased(PSB_GREEN)) {
        digitalWrite(53, LOW);
        Serial.println("lazer on");
      }
      if (ps2x.ButtonPressed(PSB_GREEN)) {
       digitalWrite(53, HIGH);
        Serial.println("lazer off");
      }
    }


    if (ps2x.ButtonPressed(PSB_RED))            //will be TRUE if button was JUST pressed
      Serial.println("Circle just pressed");

    if (ps2x.ButtonReleased(PSB_PINK))            //will be TRUE if button was JUST released
      Serial.println("Square just released");

    if (ps2x.NewButtonState(PSB_BLUE))           //will be TRUE if button was JUST pressed OR released
      Serial.println("X just changed");


    //if (ps2x.Button(PSB_L1) || ps2x.Button(PSB_R1)) // print stick values if either is TRUE
    //{
    // Serial.print("Stick Values:");
    //Serial.print(ps2x.Analog(PSS_LY), DEC); //Left stick, Y axis. Other options: LX, RY, RX
    //Serial.print(",");
    //Serial.print(ps2x.Analog(PSS_LX), DEC);
    //Serial.print(",");
    //Serial.print(ps2x.Analog(PSS_RY), DEC);
    //Serial.print(",");
    //Serial.println(ps2x.Analog(PSS_RX), DEC);
    //}

    delay(50);
  }
}
