/*tinyprintfarduino.ino
 * @file   tinyprintfarduino.ino
 *   @author    Brody Mahoney
 *   @date      5-May-2021
 *   @brief   Example TinyPrintf usage for Arduino MEGA2560
 *   
 *  The following is an example sketch showing how use printf_t
 *  and sprintf_t.
 */

#include "src/tinyprintf.h"
int seconds;

/**
 * @brief Arduino Setup Function
 *
 * @return void
 */
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  seconds = 0;
}

/**
 * @brief Arduino Loop Function
 *
 * Use printf_t and sprintf_t similarly to the tradional printf and 
 * sprintf. Be aware that in Arduino IDE, printf and sprintf are already
 * sourced. Be sure to use printf_t or sprintf_t!
 */
void loop() {
  printf_t("Time = %i seconds\n", seconds);
  char testString[20];
  sprintf_t(testString, "In Hex: 0x%x\n", seconds);
  printf_t(testString);
  delay(1000);
  seconds++;
}

