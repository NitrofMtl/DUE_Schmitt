#ifndef DUE_SCHMITT_h
#define DUE_SCHMITT_h

#include "Arduino.h"

#ifndef ARDUINO_ARCH_SAM
  	#error "This libraries is for arduino DUE only !!!"
#else

#ifndef MAX_FIELD
#define MAX_FIELD (uint32_t)-1
#endif

#define Tslow_clock 32768
#define TSLOW_CLOCK_PERIOD 0.030517578125f  // Tslow_clock period in ms ->> 1000/32768

#define SCHMITT_NON_CAPABLE_PIOA30 30
#define SCHMITT_NON_CAPABLE_PIOA31 31
#define SCHMITT_NON_CAPABLE_PIOB22 22
#define SCHMITT_UOTGVBOF_RESERVE_PIOB10 10
#define SCHMITT_UOTGID_RESERVE_PIOB11 11
#define SCHMITT_JTAG_RESERVE_PIOB28 28
#define SCHMITT_JTAG_RESERVE_PIOB29 29
#define SCHMITT_JTAG_RESERVE_PIOB30 30
#define SCHMITT_JTAG_RESERVE_PIOB31 31

#define SCHMITT_MAX_DEBOUNCE 0x3fff


//GPIO that not support filter
//Pin listed on datasheet table 3.1
#define PIOA_DISCARD_MASK ( digitalPinToBitMask(69) | digitalPinToBitMask(1) | digitalPinToBitMask(75) | digitalPinToBitMask(87) | SCHMITT_NON_CAPABLE_PIOA30 | SCHMITT_NON_CAPABLE_PIOA31 ) //PIOA 9, 26, 29, 30, 31
////PIOB 14 and 22, --> 22 not break on DUE,PIOB 10 and 11 use for USB and power USB, PIOB 28 to 31 reserve for JTAG,
#define PIOB_DISCARD_MASK ( digitalPinToBitMask(53) | 1<<SCHMITT_UOTGVBOF_RESERVE_PIOB10 | 1<<SCHMITT_UOTGID_RESERVE_PIOB11 | 1<<SCHMITT_NON_CAPABLE_PIOB22 | 1<<SCHMITT_JTAG_RESERVE_PIOB28 | 1<<SCHMITT_JTAG_RESERVE_PIOB29 | 1<<SCHMITT_JTAG_RESERVE_PIOB30 | 1<<SCHMITT_JTAG_RESERVE_PIOB31 ) 
#define PIOC_DISCARD_MASK ( digitalPinToBitMask(34) | digitalPinToBitMask(41) | digitalPinToBitMask(48) | digitalPinToBitMask(6) ) //PIOC 2, 9,15,24
#define PIOD_DISCARD_MASK ( digitalPinToBitMask(32) | -1ul << 11 )//PIOD 10, port d stop to 10.

class Schmitt_Class {
public:
	Schmitt_Class(){};
	Schmitt_Class(const Schmitt_Class&) = delete;
	Schmitt_Class& debounceEnable(uint8_t pin);
	template<typename ...Pin>
	Schmitt_Class& debounceEnable(uint8_t pin, Pin... pinx) { debounceEnable(pin); debounceEnable(pinx...); return *this; }
	Schmitt_Class& glitchEnable(uint8_t pin);
	template<typename ...Pin>
	Schmitt_Class& glitchEnable(uint8_t pin, Pin... pinx) { glitchEnable(pin); glitchEnable(pinx...); return *this; }
	Schmitt_Class& disable(uint8_t pin);
	template<typename ...Pin>
	Schmitt_Class& disable(uint8_t pin, Pin... pinx) { disable(pin); disable(pinx...); return *this; }
	Schmitt_Class& debouncePeriodSet(Pio * port, uint16_t DIV);
	Schmitt_Class& debouncePeriodSet(uint8_t pin, uint16_t DIV);
	Schmitt_Class& pioDebouncePeriod();
	float debouncePeriodGet(Pio *port);
private:
	bool validatePin(Pio *port, uint32_t pinMask);
	
};

extern Schmitt_Class Schmitt;

#endif //ifndef ARDUINO_ARCH_SAM
#endif //define DUE_SCHMITT_h