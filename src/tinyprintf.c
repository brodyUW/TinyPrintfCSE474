/*tinyprintf.c
 * @file   tinyprintf.c
 *   @author    Brody Mahoney
 *   @date      5-May-2021
 *   @brief   TinyPrintf for Arduino MEGA2560
 *   
 *  The following is the source file for CSE474 TinyPrintf
 *  version for Arduino MEGA2560, formatted for use in the
 *  Arduino IDE.
 *  
 *  Acknowledgements: This version of TinyPrintf is based on
 *  the work of a user named OPOSSUM. The original post can be
 *  found here: https://forum.43oh.com/topic/1289-tiny-printf-c-version/
 *  
 *  Some of the modifications to this code were inspired by a TinyPrintf
 *  version found at http://www.sparetimelabs.com/tinyprintf/tinyprintf.php.
 *  
 *  Modifications:
 *      1.Added a STDOUT switch to enable an sprintf. Modified printf so
 *        that printf and sprintf share a common formatting routine.
 */


#include "tinyprintf.h"
#include <stdarg.h>

static char ** spfPtr; /**< Detailed description after the member */
static unsigned char spfPtrIndex = 0;
static unsigned char stdOutDest = UARTOUT; /**< Detailed description after the member */
static void putcMEGA2560(char); /**< Detailed description after the member */
static void putc(unsigned);
static void puts(char *);
static void xtoa(unsigned long, const unsigned long*);
static void puth(unsigned);
static void formatCommon(char *, va_list); /**< Detailed description after the member */
static void stdoutSwitch(unsigned int c, unsigned char stdoutSelect); /**< Detailed description after the member */

static const unsigned long dv[] = {
//  4294967296      // 32 bit unsigned max
   1000000000,     // +0
    100000000,     // +1
     10000000,     // +2
      1000000,     // +3
       100000,     // +4
//       65535      // 16 bit unsigned max
        10000,     // +5
         1000,     // +6
          100,     // +7
           10,     // +8
            1,     // +9
};

/**
 * @brief Print formatted string to UART TX.
 *
 * printf_t is a user-accessible wrapper function for performing 
 * lightweight printf-style output to a serial console. Specific 
 * format functioning is performed in the formatCommon function.
 *      Usage:
 *          <tt>printf_t("Hex value[%u] = 0x%x\n", var1, var2);</tt>
 * @param[in] *format Characters to be sent to UART TX buffer.
 * @param[in] ... Variadic arguments to be printed.
 * @return void
 * @note Be aware that in Arduino IDE, printf and sprintf are already
 * sourced. Be sure to use printf_t or sprintf_t!
 */
void printf_t(char *format, ...)
{
   stdOutDest = UARTOUT;
   va_list a;
   va_start(a, format);
   formatCommon(format, a);
   va_end(a);
}

/**
 * @brief Write formatted data to string.
 *
 * sprintf_t is a user-accessible wrapper function for performing 
 * lightweight sprintf-style data formatting for string generation.
 * Specific format functioning is performed in the formatCommon 
 * function.
 *      Usage:
 *          <tt>char str[25];</tt>
 *          <tt>sprintf_t(str, "Hex value[%u] = 0x%x\n", var1, var2);</tt>
 * @param[out] *str Char pointer for writing formatted string.
 * @param[in] *format Characters to be sent to UART TX buffer.
 * @param[in] ... Variadic arguments to be printed.
 * @return void
 * @note Be aware that in Arduino IDE, printf and sprintf are already
 * sourced. Be sure to use printf_t or sprintf_t!
 */
void sprintf_t(char *str, char *format, ...)
{
    stdOutDest = ADDR; //Switch to pointer
    spfPtr = &str;
    spfPtrIndex = 0;
    va_list a;
    va_start(a, format);
    formatCommon(format, a);
    va_end(a);
    (*spfPtr)[spfPtrIndex] = '\0'; //Terminate string with NULL!
}

/**
 * @brief Send a character to UART TX buffer.
 *
 * putcMEGA2560 is a device specific putc function for writing a
 * character into the AtMega 2560 UART TX buffer. This function is
 * blocking and waits until the UCSR0A buffer has bit UDRE0 set to 1.
 * 
 * @param[in] c Character to be sent to UART TX buffer.
 * @return void
 * @note This function directly checks and writes to AtMega 2560 registers
 * making it device specific. This code could be used on many processors
 * by simply changing to the appropritae UART registers.
 */
static void putcMEGA2560(char c)
{
    while(!(UCSR0A&(1<<UDRE0)));
    UDR0 = c;
}

/**
 * @brief Select destination of a char output.
 *
 * stdoutSwitch provides a routing mechanism to direct chars either to 
 * memory for sprintf_t or the UART TX buffer for printf_t.
 * 
 * @param[in] c Character to be sent to destination.
 * @param[in] stdoutSelect Destination. ADDR: memory. UARTOUT: UART TX. 
 * @return void
 */
static void stdoutSwitch(unsigned int c, unsigned char stdoutSelect)
{
    switch(stdoutSelect)
    {
        case ADDR:
            (*spfPtr)[spfPtrIndex] = c;
            spfPtrIndex++;
            break;
        case UARTOUT:
        default:
            putcMEGA2560(c);
            break;
    }
}

/**
 * @brief Writes character to the stdoutSwitch function.
 *
 * Writes a character to memory or UART TX buffer through stdoutSwitch.
 * 
 * @param[in] c Character to be sent to destination.
 * @return void
 */
static void putc (unsigned c)
{
    stdoutSwitch(c, stdOutDest);
}

/**
 * @brief Writes string to stdoutSwitch through putc.
 *
 * Writes an entire string to the stdoutSwitch by repeatedly calling
 * putc until NULL char is found.
 * 
 * @param[in] *s String to be sent to memory or UART TX.
 * @return void
 */
static void puts(char *s)
{ 
    while(*s) putc(*s++);
}

/**
 * @brief Converts numeric values to strings, and writes chars to putc.
 *
 * Performs formatting for numeric variables to be represented as 
 * strings, and then repeatedly calls putc to send each char to memory
 * or UART TX buffer.
 * @param[in] x Numeric variable to be converted. 
 * @param[in] *dp Pointer to dp formatting array.
 * @return void
 */
static void xtoa(unsigned long x, const unsigned long *dp)
{
   char c;
   unsigned long d;
   if(x) {
       while(x < *dp) ++dp;
       do {
           d = *dp++;
           c = '0';
           while(x >= d) ++c, x -= d;
           putc(c);
       } while(!(d & 1));
   } else
       putc('0');
}

/**
 * @brief Writes character in hexidecimal format to stdoutSwitch.
 *
 * Writes character in hexidical format and sends to stdoutSwitch.
 * 
 * @param[in] n Hexadecimal character to be sent to memory or UART TX.
 * @return void
 */
static void puth(unsigned n)
{
   static const char hex[16] = { '0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
   putc(hex[n & 15]);
}

/**
 * @brief Common parsing function for formatting strings.
 *
 * Parses user string in printf_t or sprintf_t and formats depending on
 * user selected '%' formatting flags.
 * 
 * @param[in] *format Characters to be sent to memory or UART TX buffer.
 * @param[in] a Variable argument list containing variables to be printed.
 * @return void
 */
static void formatCommon(char *format, va_list a)
{
    char c;
    int i;
    long n;
    while(c = *format++) {
        if(c == '%') {
            switch(c = *format++) {
                case 's':                       // String
                    puts(va_arg(a, char*));
                    break;
                case 'c':                       // Char
                    putc(va_arg(a, char));
                    break;
                case 'i':                       // 16 bit Integer
                case 'u':                       // 16 bit Unsigned
                    i = va_arg(a, int);
                    if(c == 'i' && i < 0) i = -i, putc('-');
                    xtoa((unsigned)i, dv + 5);
                    break;
                case 'l':                       // 32 bit Long
                case 'n':                       // 32 bit uNsigned loNg
                    n = va_arg(a, long);
                    if(c == 'l' &&  n < 0) n = -n, putc('-');
                    xtoa((unsigned long)n, dv);
                    break;
                case 'x':                       // 16 bit heXadecimal
                    i = va_arg(a, int);
                    puth(i >> 12);
                    puth(i >> 8);
                    puth(i >> 4);
                    puth(i);
                    break;
                case 0: return;
                default: goto bad_fmt;
            }
        } else
 bad_fmt:    putc(c);
    }
}


