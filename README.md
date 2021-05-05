# TinyPrintf Arduino Mega2560 #

This repository contains a modified version of TinyPrintf and an example sketch for usage on the
Arduino Mega2560. There are many TinyPrintf solutions out there, and the general purpose is to 
replace the more "heavy-weight" printf with a smaller and faster version for use on resource
limited microcontrollers.

### How to use ###

1. Source the tinyprintf.h file with an include. It is recommended to place the tinyprintf.c and .h
  files in a "src" folder residing in the folder of your sketch. At the beginning of the sketch:
  #include "src/tinyprintf.h"

2. Use printf_t or sprintf_t like the traditional printf and sprintf. Note that in order to make printf
  "tiny" some of the more extensive formatting options were removed. The following format specifiers are
  included:

	* s: String
	* c: Char
	* i: 16 bit interger
	* u: 16 bit unsigned interger
	* l: 32 bit long
	* n: 32 bit unsigned long
	* x: 16 bit hexadecimal

**printf_t usage:**
```
unsigned char hexValue = 0xFF;
printf_t("Hex Value = 0x%x\n", hexValue);
```

**sprintf_t usage:**
```
unsigned char hexValue = 0xFF;
char str[20];
sprintf_t(str, "Hex Value = 0x%x\n", hexValue);
```

**Limitations:**
  Also notice that the ability to add leading zeros, for example, is not supported. The following will not
  produce the expected results:

```
printf_t("Value: 0x02%x\n");
```

### Acknowledgments and Modifications ###

TinyPrintf based on OPOSSUM TINYPRINTF https://forum.43oh.com/topic/1289-tiny-printf-c-version/.
Modifications: 

* Added a STDOUT switch to enable an sprintf. Modified printf so that printf and
  sprintf share a common formatting routine. Added a function pointer so that putc, which feeds
  directly into device specific UART can be decoupled from this source code, which was inspired
  by this website: http://www.sparetimelabs.com/tinyprintf/tinyprintf.php.

* Recently modified to operate specifically on ArduinoMega 2560 for EE474 class by eliminating function pointer
  and making the device specific function putcMEGA2560 from putcExt.

### In Progress ###

* Updating code to meet Code Standard.
* Debugging.
* Streamlining for Arduino MEGA2560 specific usage.
* Non-blocking version.
