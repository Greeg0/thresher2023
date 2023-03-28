#include <LiquidCrystal.h>
#include <Bounce2.h> // used to toggle button
#include <Servo.h>



#define buttonPin 8 // Assuming the joyButton pin is attached to pin7

Bounce button = Bounce(); // Create object bounce. This is used to detect a button press.

bool state; 




const int SERVO = 12; // Assuming servo is attached to pin12
const int joyVer = A1; // Vertical output to in1
const int joyHor = A2; // Horizontal output to in2

Servo myServo; // name of servo

LiquidCrystal lcd(2, 3, 4, 13, 6, 7); //initialize display, 13 was just used as the pwm of pin 5 was needed for the motor control.
bool isUpdated = true; // the display should not be updated unless it's necessary or a mess of characters happen.

int val = 0; // initial value

const int analogInPin = A0; // potentiometer
int sensorValue = 0;


const int motorPin = 5; // DC motor
const int fanPin = 11; // Fan


int fanSpeed = 0; // Fan speed.
int drumSpeed = 0; // There will be no negative speed, so this value will be unsigned.
const int maxSpeed = 255; // This may need to be changed.
int conClear = 0;
int prevConClear = 0;

int updateSpeed = 1; // This is the speed that the speeds will update at. So if the update speed is too slow, change this.

void setup() {
	lcd.begin(16,2);
	pinMode(motorPin, OUTPUT);
	pinMode(fanPin, OUTPUT);  
	myServo.attach(SERVO); // attach servo to servo object.

	pinMode(buttonPin, INPUT_PULLUP);
	button.attach(buttonPin); // Assign the button to the bounce object.

	pinMode(joyVer, INPUT);
	pinMode(joyHor, INPUT);

	pinMode(analogInPin, INPUT);

  // initalise state to false, which will start on standby.
  state = false;
}

// loop, runs either standby() or regular() depending on state.
void loop() {
    if (getButton()) { // get if button is pressed
    state = !state;     // if true, swap state
    isUpdated = true; // This is to know that since the state has changed, the LCD will require updating.
    lcd.clear(); // This will clear the LCD for on/off.
  }
  switch (state) {   // start subroutine depending on state
    case false:
      standby();
      break;
    case true:
      regular();
      break;
  }
}

bool getButton() { // button state updator
  if (button.update() && button.read() == HIGH) {
    return true;
  } else {
    return false;
  }
}

// Standby mode
void standby() {
	if (isUpdated == true){
		lcd.setCursor(6,0);
		lcd.print("OFF");
		isUpdated = false;
	}
}

// This function will return the cursor position to display and will update the unused left digit to 0.
int displayCursor(const int& basePos, const int& inVal){
	int outVal;
	if (inVal % 10 != inVal){
		outVal = basePos;
	} else {
		outVal = basePos + 1;
		lcd.setCursor(basePos, 1); // UPDATE UNUSED DIGIT TO 0.
		lcd.print(0);
	}
	return outVal;
}

void updateDisplay(){
	// This is what is updated when the adjustments update. 
	val = map(drumSpeed, 0, maxSpeed, 0, 99);
	lcd.setCursor( displayCursor(2, val) , 1);
	lcd.print(val);
	val = map(fanSpeed, 0, maxSpeed, 0, 99);
	lcd.setCursor( displayCursor(7, val) , 1);
	lcd.print(val);
	val = map(conClear, 0, 179, 0, 99);
	lcd.setCursor( displayCursor(12, val), 1);
	lcd.print(val);
}

// This is a function that takes a speed, an input from a joystick, and outputs the adjusted speed to a pin. The resting value of the joystick is around 500 so 400 and 600 are a good simple range to use.
void speedUpdate(int& speed, const int& inPin, const int& outPin){
 
	sensorValue=analogRead(inPin);

	// Adjust speed
	if (sensorValue > 600 && speed <= maxSpeed-updateSpeed){ // If shiftUp, increase speed. If the next update will make it go over the max speed, do not update. 
		speed += updateSpeed;
		updateDisplay();
	} else if (sensorValue < 400 && speed >= updateSpeed){ // If shiftDown, decrease speed. If the next update will make it go below 0, do not update.
		speed -= updateSpeed;
		updateDisplay();
	}
	analogWrite(outPin, speed);
}

void regular(){
	// Initialize regular display mode.

	if (isUpdated == true){
		lcd.setCursor(2,0);
		lcd.print("M:");
		lcd.setCursor(7,0);
		lcd.print("F:");
		lcd.setCursor(12,0);
		lcd.print("C:");
		lcd.setCursor(4,1);
		lcd.print("%");
		lcd.setCursor(9,1);
		lcd.print("%");
		lcd.setCursor(14,1);
		lcd.print("%");
		updateDisplay();
		isUpdated = false;
	}

	speedUpdate(drumSpeed, joyVer, motorPin);
	speedUpdate(fanSpeed, joyHor, fanPin);

	// updates servos, if both servos will be adjusted simutaneously, they can be both controlled 
	sensorValue = analogRead(analogInPin);
	conClear = map(sensorValue, 0, 1023, 0, 179);
	myServo.write(conClear);
	if(conClear != prevConClear){
		updateDisplay();
	}
	prevConClear = conClear;

	delay(10); // No rush.
}
