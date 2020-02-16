# DUE_Schmitt
## Use Arduino DUE internal digital pin Schmitt trigguer.

Debounce or de-glitch digital input without MCU expensive task or any additional hardware.
Glitch filter can filter a glitch 1/2 period of master clock.
Debounce filter can filter a pulse of less than 1/2 Period of a Programmable Divided Slow Clock.

### you can use on the same pin only one of glitch of debounce.

you can refer to [DUE pinout diagram](https://www.google.com/url?sa=i&url=https%3A%2F%2Fforum.arduino.cc%2Findex.php%3Ftopic%3D132130.0&psig=AOvVaw0nAT_-t24MMGK0QLDtgdW-&ust=1581963649054000&source=images&cd=vfe&ved=0CAIQjRxqFwoTCOjto_zX1ucCFQAAAAAdAAAAABAD)  find with port pin is associated with Arduino pin.

Here all DUE's pins Schmitt filter capable:
- A0 to A11
- canRX
- digital: 0, 2, 5, 7, 8, 9, 11,12,13, SDA1, SCL1
- digital: 4 ( on port C.26 )
- digital: 10 ( on port A.28 )
- digital: 22 to 31
-  digital 33 to 40
- digital 42, 43, 45, 46, 47, 49, 50, 51, 52

### Usage

- Download DUE_Schmitt library via libraries manager
- Include header into sketch
````
#include "DUE_schmitt.h"
````

- add pin you want to debounce or de-glitch
````
	// ( pin )
	Schmitt::debounceEnable(22); ///PIOB[26]
	Schmitt::glitchEnable(52); //PIOB[21]
````

- For debounce, select the debounce value needed, input if 0 to 8191, that make debonce time to 0,61 to 500ms.
````
	// Debounce period is fo each port once, could set it directly or by passing a pin member of this port
	// (port or pin, value)
	Schmitt::debouncePeriodSet(PIOB, 4096); //set with port
	Schmitt::debouncePeriodSet(48, 8191); //max //set with pin
````

- To validate your config, you can print to serial the debounce time of all port.
````
Schmitt::pioDebouncePeriod(Serial);
````

- To desale a glitch of a debounce pin.
````
	// ( pin )
	Schmitt::disable(22);
````