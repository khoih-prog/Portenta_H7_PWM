/****************************************************************************************************************************
  Portenta_H7_PWM.h
  For Portenta_H7 boards
  Written by Khoi Hoang

  Built by Khoi Hoang https://github.com/khoih-prog/Portenta_H7_PWM
  Licensed under MIT license

  Now even you use all these new 16 ISR-based timers,with their maximum interval practically unlimited (limited only by
  unsigned long miliseconds), you just consume only one Portenta_H7 STM32 timer and avoid conflicting with other cores' tasks.
  The accuracy is nearly perfect compared to software timers. The most important feature is they're ISR-based timers
  Therefore, their executions are not blocked by bad-behaving functions / tasks.
  This important feature is absolutely necessary for mission-critical tasks.

  Version: 2.0.1

  Version Modified By   Date      Comments
  ------- -----------  ---------- -----------
  1.0.0   K.Hoang      21/09/2021 Initial coding for Portenta_H7 using ArduinoCore-mbed mbed_portenta core
  2.0.0   K.Hoang      10/12/2021 Use new library code and examples
  2.0.1   K.Hoang      11/12/2021 Fix PWM_Multi example. Temporary fix polarity for HRTIM PWM
*****************************************************************************************************************************/

#pragma once

#ifndef Portenta_H7_PWM_H
#define Portenta_H7_PWM_H

#if ( ( defined(ARDUINO_PORTENTA_H7_M7) || defined(ARDUINO_PORTENTA_H7_M4) ) && defined(ARDUINO_ARCH_MBED) )
  #warning Use MBED ARDUINO_PORTENTA_H7 and LittleFS
  
  #if defined(BOARD_NAME)
    #undef BOARD_NAME
  #endif

  #if defined(CORE_CM7)
    #warning Using Portenta H7 M7 core
    #define BOARD_NAME              "PORTENTA_H7_M7"
  #else
    #warning Using Portenta H7 M4 core
    #define BOARD_NAME              "PORTENTA_H7_M4"
  #endif
  
#else
  #error This code is intended to run on the MBED ARDUINO_PORTENTA_H7 platform! Please check your Tools->Board setting. 
#endif

#ifndef PORTENTA_H7_PWM_VERSION
  #define PORTENTA_H7_PWM_VERSION       "Portenta_H7_PWM v2.0.1"
#endif


///////////////////////////////////////////

#include "Arduino.h"
#include "pinDefinitions.h"
#include "PwmOut.h"

///////////////////////////////////////////

#ifndef _PWM_LOGLEVEL_
  #define _PWM_LOGLEVEL_       1
#endif

#include "PWM_Generic_Debug.h"

///////////////////////////////////////////

bool isValidPWMPin(pin_size_t pin);

bool isValidPWMFreq(pin_size_t pin, float frequency);

bool isValidPWMDutyCycle(pin_size_t pin, float dutyCycle);

bool isUsingHRTIM(pin_size_t pin);

bool isValidPWMSettings(pin_size_t pin, float frequency, float dutyCycle);

mbed::PwmOut* setPWM(mbed::PwmOut* &pwm, pin_size_t pin, float frequency, float dutyCycle);

mbed::PwmOut* stopPWM(mbed::PwmOut* &pwm, pin_size_t pin);

#endif    // Portenta_H7_PWM_H

