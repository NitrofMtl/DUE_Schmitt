#include "DUE_schmitt.h"



void setup() {
	Serial.begin(115200);
	delay(5000);

	//put any number of pin you need to debounce (non capable or DUE reserve pin will be discarded without notice) 
	Schmitt.debounceEnable(13, 46, 48, 22); ////PIOB[27], PIOC[17], PIOB[26], 
	
	//all setup function are seft reference so call could be nested-->
	Schmitt.debounceEnable(52).debouncePeriodSet(PIOB, 4096); //PIOB[21] plus set the period of the port
	// Debounce period is fo each port once, could set it directly or by passing a pin member of this port
	Schmitt.debouncePeriodSet(PIOA, 0); //set with port, minimum value (default), 0,06ms debounce
	Schmitt.debouncePeriodSet(48, 8191); //set with pin
	Schmitt.debouncePeriodSet(PIOD, SCHMITT_MAX_DEBOUNCE);//max value 0x3fff, 1 second
	Schmitt.glitchEnable(47);
	Schmitt.pioDebouncePeriod();

	//disable on both glitch and debounce
	Schmitt.disable(22, 52, 47);
}


void loop() {
//use pin as usual


}