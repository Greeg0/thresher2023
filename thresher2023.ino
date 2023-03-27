#include <LiquidCrystal.h>
#include <Bounce2.h> // used to toggle button
#include <Servo.h>
#define buttonPin 8 // Assuming the joyButton pin is attached to pin7

Bounce button = Bounce(); // Create object bounce.

bool state;

const int SERVO = 11; // Assuming servo is attached to pin4
const int joyVer = A1; // Vertical output to in1
const int joyHor = A2; // Horizontal output to in2

Servo myServo; // name of servo

LiquidCrystal lcd(2, 3, 4, 5, 6, 7);
bool isUpdated = true; // the display should not be updated unless it's necessary or a mess of characters happen.
int val = 0; // initial value

const int analogInPin = A0; // potentiometer
int sensorValue = 0;


const int motorPin = 10; // DC motor
const int fanPin = 11; // Fan


int fanSpeed = 0; // Fan speed.
int drumSpeed = 0; // There will be no negative speed, so this value will be unsigned.
const int maxSpeed = 1023; // This may need to be changed.
int conClear = 0;
int prevConClear = 0;

int updateSpeed = 1; // This is the speed that the speeds will update at. So if the update speed is too slow, change this.

void setup() {
  Serial.begin(9600); // Begin serial monitor used to test sensors with Serial.println(foobar)
  lcd.begin(16,2);
  pinMode(motorPin, OUTPUT);
  
  myServo.attach(SERVO); // attach servo to servo object.

  pinMode(buttonPin, INPUT_PULLUP);
  button.attach(buttonPin); // Assign the button to the bounce object.

  pinMode(joyVer, INPUT);
  pinMode(joyHor, INPUT);

  pinMode(analogInPin, INPUT);

  // initalise state to false, which will start on standby.
  state = false;
}

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

void updateDisplay(){
	// This is what is updated when the adjustments update.
	lcd.setCursor(2,1);
	lcd.print(map(drumSpeed, 0, maxSpeed, 0, 99));
	lcd.setCursor(7,1);
	lcd.print(map(fanSpeed, 0, maxSpeed, 0, 99));
	lcd.setCursor(12,1);
	lcd.print(map(conClear, 0, 179, 0, 99));
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
		lcd.setCursor(5,1);
		lcd.print("%");
		lcd.setCursor(10,1);
		lcd.print("%");
		lcd.setCursor(15,1);
		lcd.print("%");
		updateDisplay();
	}
	
	// I decided to break these down in seperate functions to diagnose issues, but may revert back to the old function after its tested, but probably not.

	sensorValue=analogRead(joyVer);

	// Adjust motorspeed
	if (sensorValue > 511 && drumSpeed <= maxSpeed-updateSpeed){ // If shiftUp, increase speed. If the next update will make it go over the max speed, do not update. 
		drumSpeed += updateSpeed;
		updateDisplay();
	} else if (sensorValue < 511 && drumSpeed >= updateSpeed){ // If shiftDown, decrease speed. If the next update will make it go below 0, do not update.
		drumSpeed -= updateSpeed;
		updateDisplay();
	}
	analogWrite(motorPin, drumSpeed);
	
	sensorValue=analogRead(joyHor);
	
	// Adjust fanspeed
	if (sensorValue > 511 && fanSpeed <= maxSpeed-updateSpeed){ // If shiftUp, increase speed. If the next update will make it go over the max speed, do not update. 
		fanSpeed += updateSpeed;
		updateDisplay();
	} else if (sensorValue < 511 && fanSpeed >= updateSpeed){ // If shiftDown, decrease speed. If the next update will make it go below 0, do not update.
		fanSpeed -= updateSpeed;
		updateDisplay();
	}
	analogWrite(fanPin, fanSpeed);


	// updates servos, if both servos will be adjusted simutaneously, they can be both controlled by the same pin (i think).
	sensorValue = analogRead(analogInPin);
	conClear = map(sensorValue, 0, 1023, 0, 179);
	myServo.write(conClear);
	if(conClear != prevConClear){
		updateDisplay();
	}
	prevConClear = conClear;
}
