/*tinyprintf.h
 * @file   tinyprintf.h
 *   @author    Brody Mahoney
 *   @date      5-May-2021
 *   @brief   TinyPrintf for Arduino MEGA2560
 *   
 *  The following is a header file for CSE474 TinyPrintf
 *  version for Arduino MEGA2560, formatted for use in the
 *  Arduino IDE.
 */


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
