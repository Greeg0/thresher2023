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
	pinMode(joyButton, INPUT);
	pinMode(joyVer, INPUT);
	pinMode(joyHor, INPUT);
}


void loop () {

}
