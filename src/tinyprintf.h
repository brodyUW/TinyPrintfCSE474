#ifndef TINYPRINTF_H_
	#include <Arduino.h>
    #define TINYPRINTF_H_
    #define UARTOUT   0
    #define ADDR      1
    #ifdef __cplusplus
extern "C"
{
#endif
    void printf_t(char *, ...);
    void sprintf_t(char * str, char *format, ...);
    #ifdef __cplusplus
} // extern "C"
#endif
#endif
