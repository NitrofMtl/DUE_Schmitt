#include "DUE_schmitt.h"



void setup() {
	Serial.begin(115200);
	delay(5000);
	Schmitt.debounceEnable(13); ////PIOB[27]
	Schmitt.debounceEnable(46); ///PIOC[17]
	Schmitt.debounceEnable(48); ///PIOC[15]
	Schmitt.debounceEnable(22); ///PIOB[26]
	Schmitt.glitchEnable(52); //PIOB[21]
	//disable wor on both glitch and debounce
	Schmitt.disable(22);
	Schmitt.disable(52);

	// Debounce period is fo each port once, could set it directly or by passing a pin member of this port
	// (port or pin, value)
	Schmitt.debouncePeriodSet(PIOB, 4096); //set with port
	Schmitt.debouncePeriodSet(48, 8191); //max //set with pin
	Schmitt.pioDebouncePeriod(Serial);
}


void loop() {
//use pin as usual


}