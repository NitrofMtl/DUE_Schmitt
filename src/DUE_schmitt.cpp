#include "DUE_schmitt.h"

/*
list of register and description:
	
	- PIO_IFER (Input Filter Enable Register) W  0x0020	///1: Enables the input glitch filter on the I/O line.
	- PIO_IFDR (Input Filter Disable Register) W 0x0024	///1: Disables the input glitch filter on the I/O line.
	- PIO_IFSR (Input Filter Status Register) R  0x0028	///1: The input glitch filter is enabled on the I/O line.
				default 0

PIO can't have both glitch and deonce enable at the same time:
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
	 		#define PIO_SCDR_DIV_Msk (0x3fffu << PIO_SCDR_DIV_Pos) /**< \brief (PIO_SCDR) Slow Clock Divider Selection for Debouncing 
			#define PIO_SCDR_DIV(value) ((PIO_SCDR_DIV_Msk & ((value) << PIO_SCDR_DIV_Pos)))

//macro define in variant.h
#define digitalPinToPort(P)        ( g_APinDescription[P].pPort )
#define digitalPinToBitMask(P)     ( g_APinDescription[P].ulPin )

*/


void Schmitt::debounceEnable(uint8_t pin) {
	Pio *port = digitalPinToPort(pin);
	uint32_t pinMask = digitalPinToBitMask(pin);
	//Discard pin that are not schmmitt capable
	if ( validatePin(port, pinMask) ) {
		port->PIO_IFER |= pinMask;// Input Filter Enable Register
		port->PIO_DIFSR |= pinMask;// Debouncing Input Filter Select Register
	}
}


void Schmitt::glitchEnable(uint8_t pin) {
	Pio *port = digitalPinToPort(pin);
	uint32_t pinMask = digitalPinToBitMask(pin);
	//Discard pin that are not schmmitt capable
		if ( validatePin(port, pinMask) ) {
		port->PIO_IFER |= pinMask;// Input Filter Enable Register
		port->PIO_SCIFSR |= pinMask;// Glitch Input Filter Select Register
	}
}


void Schmitt::disable(uint8_t pin) {
	Pio *port = digitalPinToPort(pin);
	uint32_t pinMask = digitalPinToBitMask(pin);
	//Discard pin that are not schmmitt capable
	if ( validatePin(port, pinMask) ) {
		port->PIO_IFDR |= pinMask;// Input Filter Enable Register// Input Filter Enable Register
		port->PIO_SCIFSR |= pinMask;// Glitch Input Filter Select Register
	}
}


void Schmitt::debouncePeriodSet(Pio * port, uint16_t DIV) {
	port->PIO_SCDR = DIV;
}


void Schmitt::debouncePeriodSet(uint8_t pin, uint16_t DIV) {
	Pio *port = digitalPinToPort(pin);
	port->PIO_SCDR = DIV;
}

void Schmitt::pioDebouncePeriod() {
	Serial.println("Debounce period is for each PIO port : ");
	Serial.print("PIOA: "); Serial.print(debouncePeriodGet(PIOA),2);Serial.println("ms");
	Serial.print("PIOB: "); Serial.print(debouncePeriodGet(PIOB),2); Serial.println("ms");
	Serial.print("PIOC: "); Serial.print(debouncePeriodGet(PIOC),2); Serial.println("ms");
	Serial.print("PIOD: "); Serial.print(debouncePeriodGet(PIOD),2); Serial.println("ms");
	Serial.println();
}


bool Schmitt::validatePin(Pio *port, uint32_t pinMask) { //Discard pin that are not schmmitt capable
	if ( (port == PIOA) && (pinMask & PIOA_DISCARD_MASK) ) return false;
	if ( (port == PIOB) && (pinMask & PIOB_DISCARD_MASK) ) return false;
	if ( (port == PIOC) && (pinMask & PIOC_DISCARD_MASK) ) return false;
	if ( (port == PIOD) && (pinMask & PIOD_DISCARD_MASK) ) return false;
	return true;
}


float Schmitt::debouncePeriodGet(Pio *port) {
	return TSLOW_CLOCK_PERIOD * ( port->PIO_SCDR +1 ) * 2;
}


