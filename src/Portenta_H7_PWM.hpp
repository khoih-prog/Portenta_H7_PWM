/****************************************************************************************************************************
  Portenta_H7_PWM.hpp
  For Portenta_H7 boards
  Written by Khoi Hoang

  Built by Khoi Hoang https://github.com/khoih-prog/Portenta_H7_PWM
  Licensed under MIT license

  Version: 2.1.0

  Version Modified By   Date      Comments
  ------- -----------  ---------- -----------
  1.0.0   K.Hoang      21/09/2021 Initial coding for Portenta_H7 using ArduinoCore-mbed mbed_portenta core
  2.0.0   K.Hoang      10/12/2021 Use new library code and examples
  2.0.1   K.Hoang      11/12/2021 Fix PWM_Multi example. Temporary fix polarity for HRTIM PWM
  2.0.2   K.Hoang      07/02/2022 Convert to h-only. Optimize code.
  2.0.3   K.Hoang      21/01/2023 Add `PWM_StepperControl` example
  2.1.0   K.Hoang      24/01/2023 Add `PWM_manual` example and related function. Optimize for speed
*****************************************************************************************************************************/

#pragma once

#ifndef Portenta_H7_PWM_HPP
#define Portenta_H7_PWM_HPP

#if ( ( defined(ARDUINO_PORTENTA_H7_M7) || defined(ARDUINO_PORTENTA_H7_M4) ) && defined(ARDUINO_ARCH_MBED) )
  #warning Use MBED ARDUINO_PORTENTA_H7 and LittleFS
#else
  #error This code is intended to run on the MBED ARDUINO_PORTENTA_H7 platform! Please check your Tools->Board setting. 
#endif

#ifndef PORTENTA_H7_PWM_VERSION
  #define PORTENTA_H7_PWM_VERSION             "Portenta_H7_PWM v2.1.0"
  
  #define PORTENTA_H7_PWM_VERSION_MAJOR       2
  #define PORTENTA_H7_PWM_VERSION_MINOR       1
  #define PORTENTA_H7_PWM_VERSION_PATCH       0

  #define PORTENTA_H7_PWM_VERSION_INT         2001000
#endif


///////////////////////////////////////////

#include "Arduino.h"
#include "pinDefinitions.h"
#include "PwmOut.h"

///////////////////////////////////////////
  
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
  
///////////////////////////////////////////

#ifndef _PWM_LOGLEVEL_
  #define _PWM_LOGLEVEL_       1
#endif

#include "PWM_Generic_Debug.h"

///////////////////////////////////////////

bool isValidPWMPin(const pin_size_t& pin);

bool isValidPWMFreq(const pin_size_t& pin, const float& frequency);

bool isValidPWMDutyCycle(const pin_size_t& pin, const float& dutyCycle);

bool isUsingHRTIM(const pin_size_t& pin);

bool isValidPWMSettings(const pin_size_t& pin, const float& frequency, const float& dutyCycle);

mbed::PwmOut* setPWM(mbed::PwmOut* &pwm, const pin_size_t& pin, const float& frequency, const float& dutyCycle);

mbed::PwmOut* setPWM_DCPercentage_manual(mbed::PwmOut* &pwm, const pin_size_t& pin, const float& DCPercentage);

mbed::PwmOut* stopPWM(mbed::PwmOut* &pwm, const pin_size_t& pin);

#endif    // Portenta_H7_PWM_HPP

