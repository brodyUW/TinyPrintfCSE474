/***********************************
Example:Tiny Printf Usage
1. Include "src/tinyprintf.h".
2. Use Serial.begin as normally done.
************************************/

#include "src/tinyprintf.h"
int seconds;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  seconds = 0;
}

void loop() {
  printf_t("Time = %i seconds\n", seconds);
  char testString[20];
  sprintf_t(testString, "In Hex: 0x%x\n", seconds);
  printf_t(testString);
  delay(1000);
  seconds++;
}

