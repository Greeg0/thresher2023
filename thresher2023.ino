/*
#include <LiquidCrystal.h>
const int joyButton = 12;
const int joyVer = A0
const int joyHor = A1

bool deltaToggle = false;
bool toggle = false;
bool button = false;

// returns true to toggle.
bool toggler(){
	button=digitalRead(joyButton); // 0/1 can be assigned to a boolean.
	bool toggleOut = false;
	if (button == true && deltaToggle == true){ // if button is pressed and held, deltaToggle will be false. 
		deltaToggle = false; 
		toggleOut = true;
	} else {
		deltaToggle = !button; // If button is NOT pressed, then deltaToggle will be true and will toggle next time it's pressed. If it's pressed and held, it'll remain false. 
		toggleOut = false;
	}
	return toggleOut; 
}
void setup () {
	serial.begin(9600);
	pinMode(joyButton, INPUT);
	pinMode(joyVer, INPUT);
	pinMode(joyHor, INPUT);
}
void loop () {
}
*/
#include <Bounce2.h> // Library
#include <Servo.h>
#define buttonPin 7 // Assuming the joyButton pin is attached to pin7

Bounce button = Bounce(); // Part of the library
bool state;

const int SERVO = 4; // Assuming servo is attached to pin4
const int joyVer = A1; // Vertical output to in1
const int joyHor = A2; // Horizontal output to in2

Servo myServo; // name of servo

int val = 0; // initial value

int analogInPin = A0; // potentiometer
int sensorValue = 0;
int outputValue = 0;
int motorPin = 3; // DC motor

void setup() {
  Serial.begin(9600); // Not necessary
  pinMode(motorPin, OUTPUT);
  myServo.attach(SERVO);
  pinMode(buttonPin, INPUT_PULLUP); // Part of library
  button.attach(buttonPin);
  pinMode(joyVer, INPUT);
  pinMode(joyHor, INPUT);
  state = false;
}

void loop() {
    if (getButton()) { // get if button is pressed
    state = !state;     // if true, swap state
  }
  switch (state) {   // start subroutine depending on state
    case false:
      subRoutineOne();
      break;
    case true:
      subRoutineTwo();
      break;
  }
}

bool getButton() { // button state updator
  if (button.update() && button.read() == LOW) {
    return true;
  } else {
    return false;
  }
}

// subRoutineOne
void subRoutineOne() { 
  sensorValue = analogRead(analogInPin);
  outputValue = map(sensorValue, 0, 1023, 0, 255); // maps DC motor
  analogWrite(motorPin, sensorValue);
  delay(10);
}

// subRoutineTwo
void subRoutineTwo() {
  val = analogRead(analogInPin);
  val = map(val, 0, 1023, 0, 179); // maps servo
  myServo.write(val);
  delay(15);
}
