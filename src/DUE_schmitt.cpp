#include "DUE_schmitt.h"

/*
list of register and description:
	
	- PIO_IFER (Input Filter Enable Register) W  0x0020	///1: Enables the input glitch filter on the I/O line.
	- PIO_IFDR (Input Filter Disable Register) W 0x0024	///1: Disables the input glitch filter on the I/O line.
	- PIO_IFSR (Input Filter Status Register) R  0x0028	///1: Read The input glitch filter state on the I/O line.
				default 0

PIO can't have both glitch and debonce enable at the same time:
	- PIO_SCIFSR (System Clock Glitch Input Filter Select Register) W  0x0080 ///1: The Glitch Filter is able to filter glitches with a duration < Tmck/2.
	- PIO_DIFSR (Debouncing Input Filter Select Register)			W  0x0084 ///1: The Debouncing Filter is able to filter pulses with a duration < Tdiv_slclk/2
	- PIO_IFDGSR (Glitch or Debouncing Input Filter Selection Status Register) R, default 0  0x0088
			///0: The Glitch Filter is able to filter glitches with a duration < Tmck2.
			///1: The Debouncing Filter is able to filter pulses with a duration < Tdiv_slclk/2.

	- PIO_SCDR (Slow Clock Divider Register) RW default 0  0x008C DIV[x] -> 0-13
	  Tdiv_slclk = ((DIV+1)*2)*Tslow_clock --> datasheet p660 //slow_clock 32768 Hz
	 		For the debouncing filter, the Period of the Divided Slow Clock is performed by writing in the DIV field of the PIO_SCDR register

	 		//define in component_pio.h
	 		#define PIO_SCDR_DIV_Pos 0
	 		#define PIO_SCDR_DIV_Msk (0x3fffu << PIO_SCDR_DIV_Pos) //< \brief (PIO_SCDR) Slow Clock Divider Selection for Debouncing 
			#define PIO_SCDR_DIV(value) ((PIO_SCDR_DIV_Msk & ((value) << PIO_SCDR_DIV_Pos)))

//macro define in variant.h
#define digitalPinToPort(P)        ( g_APinDescription[P].pPort )
#define digitalPinToBitMask(P)     ( g_APinDescription[P].ulPin )

*/


Schmitt_Class& Schmitt_Class::debounceEnable(uint8_t pin) {
	Pio *port = digitalPinToPort(pin);
	uint32_t pinMask = digitalPinToBitMask(pin);
	//Discard pin that are not schmmitt capable
	if ( !validatePin(port, pinMask) ) return *this;
	port->PIO_IFER |= pinMask;// Input Filter Enable Register
	port->PIO_DIFSR |= pinMask;// Debouncing Input Filter Select Register
	return *this;
}


Schmitt_Class& Schmitt_Class::glitchEnable(uint8_t pin) {
	Pio *port = digitalPinToPort(pin);
	uint32_t pinMask = digitalPinToBitMask(pin);
	//Discard pin that are not schmmitt capable
	if ( !validatePin(port, pinMask) ) return *this;
	port->PIO_IFER |= pinMask;// Input Filter Enable Register
	port->PIO_SCIFSR |= pinMask;// Glitch Input Filter Select Register
	return *this;
}


Schmitt_Class& Schmitt_Class::disable(uint8_t pin) {
	Pio *port = digitalPinToPort(pin);
	uint32_t pinMask = digitalPinToBitMask(pin);
	//Discard pin that are not schmmitt capable or reserve use on DUE board
	if ( !validatePin(port, pinMask) ) return *this;
	port->PIO_IFDR |= pinMask;// Input Filter Enable Register// Input Filter Enable Register
	port->PIO_SCIFSR |= pinMask;// Glitch Input Filter Select Register
	return *this;
}


Schmitt_Class& Schmitt_Class::debouncePeriodSet(Pio * port, uint16_t DIV) {
	port->PIO_SCDR = DIV;
	return *this;
}


Schmitt_Class& Schmitt_Class::debouncePeriodSet(uint8_t pin, uint16_t DIV) {
	Pio *port = digitalPinToPort(pin);
	port->PIO_SCDR = DIV;
	return *this;
}

Schmitt_Class& Schmitt_Class::pioDebouncePeriod() {
	char buffer[100];
	sprintf(buffer, 
		"Debounce period is for each PIO port : \nPIOA: %.2fms\nPIOB: %.2fms\nPIOC: %.2fms\nPIOD: %.2fms\n\n",
		 debouncePeriodGet(PIOA), debouncePeriodGet(PIOB), debouncePeriodGet(PIOC), debouncePeriodGet(PIOD));
	Serial.print(buffer);
	return *this;
}


bool Schmitt_Class::validatePin(Pio *port, uint32_t pinMask) { //Discard pin that are not schmmitt capable
	if ( (port == PIOA) && (pinMask & PIOA_DISCARD_MASK) ) return false;
	if ( (port == PIOB) && (pinMask & PIOB_DISCARD_MASK) ) return false;
	if ( (port == PIOC) && (pinMask & PIOC_DISCARD_MASK) ) return false;
	if ( (port == PIOD) && (pinMask & PIOD_DISCARD_MASK) ) return false;
	return true;
}


float Schmitt_Class::debouncePeriodGet(Pio *port) {
	return TSLOW_CLOCK_PERIOD * ( port->PIO_SCDR +1 ) * 2;
}

Schmitt_Class Schmitt;

