/****************************************************************************************************************************
  multiFileProject.ino
  
  For Portenta_H7 boards
  Written by Khoi Hoang

  Built by Khoi Hoang https://github.com/khoih-prog/Portenta_H7_PWM
  Licensed under MIT license
*****************************************************************************************************************************/

// To demo how to include files in multi-file Projects

#if !( defined(ARDUINO_PORTENTA_H7_M7) || defined(ARDUINO_PORTENTA_H7_M4) )
  #error For Portenta_H7 only
#endif

#define PORTENTA_H7_PWM_VERSION_MIN_TARGET      F("Teensy_Slow_PWM v1.2.0")
#define PORTENTA_H7_PWM_VERSION_MIN             1002000

#include "multiFileProject.h"

// To be included only in main(), .ino with setup() to avoid `Multiple Definitions` Linker Error
#include "Portenta_H7_PWM.h"

void setup() 
{
  Serial.begin(115200);
  while (!Serial);
  
  Serial.println("\nStart multiFileProject");
  Serial.println(PORTENTA_H7_PWM_VERSION);

#if defined(PORTENTA_H7_PWM_VERSION_MIN)
  if (PORTENTA_H7_PWM_VERSION_INT < PORTENTA_H7_PWM_VERSION_MIN)
  {
    Serial.print("Warning. Must use this example on Version equal or later than : ");
    Serial.println(PORTENTA_H7_PWM_VERSION_MIN_TARGET);
  }
#endif
}

void loop() 
{
  // put your main code here, to run repeatedly:
}
