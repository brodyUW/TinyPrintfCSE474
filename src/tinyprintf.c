/* TinyPrintf based on OPOSSUM TINYPRINTF https://forum.43oh.com/topic/1289-tiny-printf-c-version/
   Modifications: Added a STDOUT switch to enable an sprintf. Modified printf so that printf and
   sprintf share a common formatting routine. Added a function pointer so that putc, which feeds
   directly into device specific UART can be decoupled from this source code, which was inspired
   by this website: http://www.sparetimelabs.com/tinyprintf/tinyprintf.php.
   
   Recently modified to operate specifically on ArduinoMega 2560 by eliminating function pointer
   and making the device specific function putcMEGA2560 from putcExt.
*/


#include "tinyprintf.h"
#include <stdarg.h>


static char ** spfPtr;
static unsigned char spfPtrIndex = 0;
static unsigned char stdOutDest = UARTOUT; //default
static void putcMEGA2560(char); //Renamed to putcExt_Int from putcExt
static void putc(unsigned);
static void puts(char *);
static void xtoa(unsigned long, const unsigned long*);
static void puth(unsigned);
static void formatCommon(char *, va_list);
static void stdoutSwitch(unsigned int c, unsigned char stdoutSelect);

void putcMEGA2560(char c)
{
	while(!(UCSR0A&(1<<UDRE0)));
    UDR0 = c;
}
void printf_t(char *format, ...)
{
   stdOutDest = UARTOUT;
   va_list a;
   va_start(a, format);
   formatCommon(format, a);
   va_end(a);
}

void sprintf_t(char * str, char *format, ...)
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

//For printf, send to UART
//sprintf, send to memory
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

static void putc (unsigned c)
{
    stdoutSwitch(c, stdOutDest);
}

static void puts(char *s) { while(*s) putc(*s++); }

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

static void puth(unsigned n)
{
   static const char hex[16] = { '0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
   putc(hex[n & 15]);
}

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


