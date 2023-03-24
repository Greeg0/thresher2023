#include <LiquidCrystal.h>
#include <Bounce2.h> // used to toggle button
#include <Servo.h>
#define buttonPin 7 // Assuming the joyButton pin is attached to pin7

Bounce button = Bounce(); // Create object bounce.
bool state;

const int SERVO = 4; // Assuming servo is attached to pin4
const int joyVer = A1; // Vertical output to in1
const int joyHor = A2; // Horizontal output to in2

Servo myServo; // name of servo

int val = 0; // initial value

const int analogInPin = A0; // potentiometer
int sensorValue = 0;
int outputValue = 0;


const int motorPin = 3; // DC motor
const int fanPin = 1; // Fan


unsigned int fanSpeed = 0; // Fan speed.
unsigned int drumSpeed = 0; // There will be no negative speed, so this value will be unsigned.
const int maxSpeed = 1023; // This may need to be changed.

int updateSpeed = 1; // This is the speed that the speeds will update at. So if the update speed is too slow, change this.

void setup() {
  Serial.begin(9600); // Begin serial monitor used to test sensors with Serial.println(foobar)

  pinMode(motorPin, OUTPUT);
  
  myServo.attach(SERVO); // attach servo to servo object.

  pinMode(buttonPin, INPUT_PULLUP);
  button.attach(buttonPin); // Assign the button to the bounce object.

  pinMode(joyVer, INPUT);
  pinMode(joyHor, INPUT);

  // initalise state to false, which will start on standby.
  state = false;
}

void loop() {
    if (getButton()) { // get if button is pressed
    state = !state;     // if true, swap state
  }
  switch (state) {   // start subroutine depending on state
    case false:
      standby();
      break;
    case true:
      main();
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

// Standby mode
void standby() { 
	// set LCD to low brightness to say "STANDBY, press JOYSTICK to turn on".
}

// speedUpdate
void speedUpdate(unsigned &int speed, const &int adjuster, const &int outPin) {
	int input;
	input = analogRead(adjuster);
	if (input > 511 && speed <= maxSpeed-updateSpeed){ // If shiftUp, increase speed. If the next update will make it go over the max speed, do not update. 
		speed += updateSpeed;
		updateDisplay();
	} else if (input < 511 && speed >= updateSpeed){ // If shiftDown, decrease speed. If the next update will make it go below 0, do not update.
		speed -= updateSpeed;
		updateDisplay();
	}
	digitalWrite
}

void updateDisplay(){
	// code to update display.
}

void main(){
	speedUpdate(drumSpeed, joyVer, motorPin);
	speedUpdate(fanSpeed, joyHor, fanPin);

	// updates servos, if both servos will be adjusted simutaneously, they can be both controlled by the same pin (i think).
	sensorValue = analogRead(analogInPin);
	sensorValue = map(sensorValue, 0, 1023, 0, 179);
	myServo.write(sensorValue);

}
