#ifndef DUE_SCHMITT_h
#define DUE_SCHMITT_h

#include "Arduino.h"

#if defined(ARDUINO_ARCH_AVR)
  	#error "This libraries is for arduino DUE only !!!"
#elif defined(ARDUINO_ARCH_SAM)
  // SAM-specific code

#ifndef MAX_FIELD
#define MAX_FIELD (uint32_t)-1
#endif

#define Tslow_clock 32768
#define TSLOW_CLOCK_PERIOD 0.030517578125f  // Tslow_clock period in ms ->> 1000/32768


#define BIT(x) 1<<x
#define PIOA_DISCARD_MASK (BIT(9) | BIT(26) | BIT(29) | BIT(30) | BIT(31))
#define PIOB_DISCARD_MASK (BIT(14) | BIT(22))
#define PIOC_DISCARD_MASK (BIT(2) | BIT(9) | BIT(15) | BIT(24))
#define PIOD_DISCARD_MASK (BIT(10) | BIT(30))

class Schmitt_class {
public:
	static void debounceEnable(uint8_t pin);
	static void glitchEnable(uint8_t pin);
	static void disable(uint8_t pin);
	static void debouncePeriodSet(Pio * port, uint16_t DIV);
	static void debouncePeriodSet(uint8_t pin, uint16_t DIV);
	static void pioDebouncePeriod(HardwareSerial& stream);
private:
	static bool validatePin(Pio *port, uint32_t pinMask);
	static float debouncePeriodGet(Pio *port);
};

extern Schmitt_class Schmitt;


#endif
#endif