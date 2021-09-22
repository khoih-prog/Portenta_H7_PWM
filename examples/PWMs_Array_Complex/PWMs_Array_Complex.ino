/****************************************************************************************************************************
  PWMs_Array_Complex.ino
  For Portenta_H7 boards
  Written by Khoi Hoang

  Built by Khoi Hoang https://github.com/khoih-prog/Portenta_H7_PWM
  Licensed under MIT license

  Now even you use all these new 16 ISR-based timers,with their maximum interval practically unlimited (limited only by
  unsigned long miliseconds), you just consume only one Portenta_H7 STM32 timer and avoid conflicting with other cores' tasks.
  The accuracy is nearly perfect compared to software timers. The most important feature is they're ISR-based timers
  Therefore, their executions are not blocked by bad-behaving functions / tasks.
  This important feature is absolutely necessary for mission-critical tasks.

  Version: 1.0.0

  Version Modified By   Date      Comments
  ------- -----------  ---------- -----------
  1.0.0   K.Hoang      21/09/2021 Initial coding for Portenta_H7 using ArduinoCore-mbed mbed_portenta core
*****************************************************************************************************************************/

/*
   In cores/arduino/stm32/PeripheralPins.h => #define PinMap_PWM PinMap_TIM

   /////////////////////////////////////////////////////////////////////////////

   In cores/arduino/stm32/PinNamesTypes.h

   #define STM_PIN_DEFINE(FUNC_OD, PUPD, AFNUM)  ((int)(FUNC_OD) | ((PUPD  & STM_PIN_PUPD_MASK) << STM_PIN_PUPD_SHIFT) | ((AFNUM & STM_PIN_AFNUM_MASK) << STM_PIN_AFNUM_SHIFT))

   #define STM_PIN_DEFINE_EXT(FUNC_OD, PUPD, AFNUM, CHAN, INV)  ((int)(FUNC_OD) | ((PUPD   & STM_PIN_PUPD_MASK) << STM_PIN_PUPD_SHIFT) |\
                       ((AFNUM  & STM_PIN_AFNUM_MASK) << STM_PIN_AFNUM_SHIFT) | (CHAN   & STM_PIN_CHAN_MASK) << STM_PIN_CHAN_SHIFT) | ((INV    & STM_PIN_INV_MASK) << STM_PIN_INV_SHIFT))

   ///////////////////////////////////////////////

   #define STM_PIN_DATA(FUNC_OD, PUPD, AFNUM)                           STM_PIN_DEFINE(FUNC_OD, PUPD, AFNUM)
   #define STM_PIN_DATA_EXT(FUNC_OD, PUPD, AFNUM, CHANNEL, INVERTED)    STM_PIN_DEFINE_EXT(FUNC_OD, PUPD, AFNUM, CHANNEL, INVERTED)
*/

/*
   // For example, in ./2.0.0/variants/STM32H7xx/H747ZIY_H757ZIY/PeripheralPins.c

  WEAK const PinMap PinMap_TIM[] = {
  {PA_0,       TIM2,  STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF1_TIM2, 1, 0)}, // TIM2_CH1
  {PA_0_ALT1,  TIM5,  STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF2_TIM5, 1, 0)}, // TIM5_CH1
  {PA_1,       TIM2,  STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF1_TIM2, 2, 0)}, // TIM2_CH2
  {PA_1_ALT1,  TIM5,  STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF2_TIM5, 2, 0)}, // TIM5_CH2
  {PA_1_ALT2,  TIM15, STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF4_TIM15, 1, 1)}, // TIM15_CH1N
  {PA_2,       TIM2,  STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF1_TIM2, 3, 0)}, // TIM2_CH3
  {PA_2_ALT1,  TIM5,  STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF2_TIM5, 3, 0)}, // TIM5_CH3
  {PA_2_ALT2,  TIM15, STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF4_TIM15, 1, 0)}, // TIM15_CH1
  {PA_3,       TIM2,  STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF1_TIM2, 4, 0)}, // TIM2_CH4
  {PA_3_ALT1,  TIM5,  STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF2_TIM5, 4, 0)}, // TIM5_CH4
  {PA_3_ALT2,  TIM15, STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF4_TIM15, 2, 0)}, // TIM15_CH2
  {PA_5,       TIM2,  STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF1_TIM2, 1, 0)}, // TIM2_CH1
  {PA_5_ALT1,  TIM8,  STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF3_TIM8, 1, 1)}, // TIM8_CH1N
  {PA_6,       TIM3,  STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF2_TIM3, 1, 0)}, // TIM3_CH1
  {PA_6_ALT1,  TIM13, STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF9_TIM13, 1, 0)}, // TIM13_CH1
  {PA_7,       TIM1,  STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF1_TIM1, 1, 1)}, // TIM1_CH1N
  {PA_7_ALT1,  TIM3,  STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF2_TIM3, 2, 0)}, // TIM3_CH2
  {PA_7_ALT2,  TIM8,  STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF3_TIM8, 1, 1)}, // TIM8_CH1N
  {PA_7_ALT3,  TIM14, STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF9_TIM14, 1, 0)}, // TIM14_CH1
  {PA_8,       TIM1,  STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF1_TIM1, 1, 0)}, // TIM1_CH1
  {PA_9,       TIM1,  STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF1_TIM1, 2, 0)}, // TIM1_CH2
  {PA_10,      TIM1,  STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF1_TIM1, 3, 0)}, // TIM1_CH3
  {PA_11,      TIM1,  STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF1_TIM1, 4, 0)}, // TIM1_CH4
  {PA_15,      TIM2,  STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF1_TIM2, 1, 0)}, // TIM2_CH1
  {PB_0,       TIM1,  STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF1_TIM1, 2, 1)}, // TIM1_CH2N
  {PB_0_ALT1,  TIM3,  STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF2_TIM3, 3, 0)}, // TIM3_CH3
  {PB_0_ALT2,  TIM8,  STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF3_TIM8, 2, 1)}, // TIM8_CH2N
  {PB_1,       TIM1,  STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF1_TIM1, 3, 1)}, // TIM1_CH3N
  {PB_1_ALT1,  TIM3,  STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF2_TIM3, 4, 0)}, // TIM3_CH4
  {PB_1_ALT2,  TIM8,  STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF3_TIM8, 3, 1)}, // TIM8_CH3N
  {PB_3,       TIM2,  STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF1_TIM2, 2, 0)}, // TIM2_CH2
  {PB_4,       TIM3,  STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF2_TIM3, 1, 0)}, // TIM3_CH1
  {PB_5,       TIM3,  STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF2_TIM3, 2, 0)}, // TIM3_CH2
  {PB_6,       TIM4,  STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF2_TIM4, 1, 0)}, // TIM4_CH1
  {PB_6_ALT1,  TIM16, STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF1_TIM16, 1, 1)}, // TIM16_CH1N
  {PB_7,       TIM4,  STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF2_TIM4, 2, 0)}, // TIM4_CH2
  {PB_7_ALT1,  TIM17, STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF1_TIM17, 1, 1)}, // TIM17_CH1N
  {PB_8,       TIM4,  STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF2_TIM4, 3, 0)}, // TIM4_CH3
  {PB_8_ALT1,  TIM16, STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF1_TIM16, 1, 0)}, // TIM16_CH1
  {PB_9,       TIM4,  STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF2_TIM4, 4, 0)}, // TIM4_CH4
  {PB_9_ALT1,  TIM17, STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF1_TIM17, 1, 0)}, // TIM17_CH1
  {PB_10,      TIM2,  STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF1_TIM2, 3, 0)}, // TIM2_CH3
  {PB_11,      TIM2,  STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF1_TIM2, 4, 0)}, // TIM2_CH4
  {PB_13,      TIM1,  STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF1_TIM1, 1, 1)}, // TIM1_CH1N
  {PB_14,      TIM1,  STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF1_TIM1, 2, 1)}, // TIM1_CH2N
  {PB_14_ALT1, TIM8,  STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF3_TIM8, 2, 1)}, // TIM8_CH2N
  {PB_14_ALT2, TIM12, STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF2_TIM12, 1, 0)}, // TIM12_CH1
  {PB_15,      TIM1,  STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF1_TIM1, 3, 1)}, // TIM1_CH3N
  {PB_15_ALT1, TIM8,  STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF3_TIM8, 3, 1)}, // TIM8_CH3N
  {PB_15_ALT2, TIM12, STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF2_TIM12, 2, 0)}, // TIM12_CH2
  {PC_6,       TIM3,  STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF2_TIM3, 1, 0)}, // TIM3_CH1
  {PC_6_ALT1,  TIM8,  STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF3_TIM8, 1, 0)}, // TIM8_CH1
  {PC_7,       TIM3,  STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF2_TIM3, 2, 0)}, // TIM3_CH2
  {PC_7_ALT1,  TIM8,  STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF3_TIM8, 2, 0)}, // TIM8_CH2
  {PC_8,       TIM3,  STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF2_TIM3, 3, 0)}, // TIM3_CH3
  {PC_8_ALT1,  TIM8,  STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF3_TIM8, 3, 0)}, // TIM8_CH3
  {PC_9,       TIM3,  STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF2_TIM3, 4, 0)}, // TIM3_CH4
  {PC_9_ALT1,  TIM8,  STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF3_TIM8, 4, 0)}, // TIM8_CH4
  {PD_12,      TIM4,  STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF2_TIM4, 1, 0)}, // TIM4_CH1
  {PD_13,      TIM4,  STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF2_TIM4, 2, 0)}, // TIM4_CH2
  {PD_14,      TIM4,  STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF2_TIM4, 3, 0)}, // TIM4_CH3
  {PD_15,      TIM4,  STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF2_TIM4, 4, 0)}, // TIM4_CH4
  {PE_4,       TIM15, STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF4_TIM15, 1, 1)}, // TIM15_CH1N
  {PE_5,       TIM15, STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF4_TIM15, 1, 0)}, // TIM15_CH1
  {PE_6,       TIM15, STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF4_TIM15, 2, 0)}, // TIM15_CH2
  {PE_8,       TIM1,  STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF1_TIM1, 1, 1)}, // TIM1_CH1N
  {PE_9,       TIM1,  STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF1_TIM1, 1, 0)}, // TIM1_CH1
  {PE_10,      TIM1,  STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF1_TIM1, 2, 1)}, // TIM1_CH2N
  {PE_11,      TIM1,  STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF1_TIM1, 2, 0)}, // TIM1_CH2
  {PE_12,      TIM1,  STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF1_TIM1, 3, 1)}, // TIM1_CH3N
  {PE_13,      TIM1,  STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF1_TIM1, 3, 0)}, // TIM1_CH3
  {PE_14,      TIM1,  STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF1_TIM1, 4, 0)}, // TIM1_CH4
  {NC,         NP,    0}
  };
  #endif
*/
//

#if !( defined(ARDUINO_PORTENTA_H7_M7) || defined(ARDUINO_PORTENTA_H7_M4) )
  #error For Portenta_H7 only
#endif

// These define's must be placed at the beginning before #include "ESP32_PWM.h"
// _PWM_LOGLEVEL_ from 0 to 4
// Don't define _PWM_LOGLEVEL_ > 0. Only for special ISR debugging only. Can hang the system.
#define _PWM_LOGLEVEL_      4

#define USING_MICROS_RESOLUTION       true    //false 

#include "Portenta_H7_PWM.h"

#define LED_ON        LOW
#define LED_OFF       HIGH

#include <SimpleTimer.h>              // https://github.com/jfturcot/SimpleTimer

#define HW_TIMER_INTERVAL_US      20L

volatile uint64_t startMicros = 0;


/////////////////////////////////////////////////

//D0,  Instance = 0x40010400, channel = 3, TimerIndex = 7
//D1,  Instance = 0x40010000, channel = 1, TimerIndex = 0
//D2,  Instance = 0x40010000, channel = 2, TimerIndex = 0
//D4,  Instance = 0x40000400, channel = 2, TimerIndex = 2
//D5,  Instance = 0x40000400, channel = 1, TimerIndex = 2
// A0-A6
//A0/D15, Instance = 0x40010400, channel = 3, TimerIndex = 7
//A1/D16, Instance = 0x40010000, channel = 1, TimerIndex = 0

// Can't use same TimerIndex again, e.g., the D1 and D2 can't be use together as the latter called will override
// That's why D0, D1 and D4 (using TimerIndex 7,0 ans 2) are OK together

// Only OK for D0, D1, D2, D4 and D5, PA_0C(D15/A0), PA_1C(D16/A1), 
// D3, D6, D7, D8, D9, D10, D11, D12, D13, D14, D17(PC_2C/A2), D18(PC_3C/3), PC2(D19/A4) LEDG, LEDB not OK
#define pin0    D0
#define pin1    D1
#define pin2    D2    //D2
#define pin3    D4
#define pin4    D5
#define pin5    PA_0C
#define pin6    PA_1C  

//////////////////////////////////////////////////////

uint32_t pins[]       = { pin0, pin1, pin2, pin3 };

#define NUM_OF_PINS             ( sizeof(pins) / sizeof(uint32_t) )

#define NUMBER_ISR_PWMS         NUM_OF_PINS

//TIM_TypeDef *TimerInstance[] = { TIM8, TIM12, TIM13, TIM14 };
//TIM_TypeDef *TimerInstance[] = { TIM1, TIM4, TIM7, TIM8 };
TIM_TypeDef *TimerInstance[] = { TIM1, TIM4, TIM8, TIM12 };

volatile uint32_t callbackTime[] = { 0, 0, 0, 0 };

//////////////////////////////////////////////////////

#define USE_COMPLEX_STRUCT      true

//////////////////////////////////////////////////////

#if USE_COMPLEX_STRUCT

typedef struct
{
  uint32_t      PWM_Pin;

  callback_function_t irqCallbackStartFunc;
  callback_function_t irqCallbackStopFunc;

  uint32_t      PWM_Freq; 
  uint32_t      PWM_DutyCycle;
  
  uint64_t      deltaMicrosStart;
  uint64_t      previousMicrosStart;

  uint64_t      deltaMicrosStop;
  uint64_t      previousMicrosStop;
  
} ISR_PWM_Data;

// Avoid doing something fancy in ISR, for example Serial.print()
// The pure simple Serial.prints here are just for demonstration and testing. Must be eliminate in working environment
// Or you can get this run-time error / crash

void doingSomethingStart(int index);

void doingSomethingStop(int index);

#else   // #if USE_COMPLEX_STRUCT

volatile uint64_t deltaMicrosStart    [NUMBER_ISR_PWMS] = { 0, 0, 0, 0 };
volatile uint64_t previousMicrosStart [NUMBER_ISR_PWMS] = { 0, 0, 0, 0 };

volatile uint64_t deltaMicrosStop     [NUMBER_ISR_PWMS] = { 0, 0, 0, 0 };
volatile uint64_t previousMicrosStop  [NUMBER_ISR_PWMS] = { 0, 0, 0, 0 };

// You can assign pins here. Be carefull to select good pin to use or crash
uint32_t PWM_Pin[NUMBER_ISR_PWMS] =
{
   pin0, pin1, pin2, pin3
};

// You can assign any interval for any timer here, in Hz
uint32_t PWM_Freq[NUMBER_ISR_PWMS] =
{
  2, 5, 50, 100
};

// You can assign any interval for any timer here, in milliseconds
uint32_t PWM_DutyCycle[NUMBER_ISR_PWMS] =
{
  20, 30, 50, 70
};

void doingSomethingStart(int index)
{
  uint64_t currentMicros  = micros();

  deltaMicrosStart[index]    = currentMicros - previousMicrosStart[index];
  previousMicrosStart[index] = currentMicros;
}

void doingSomethingStop(int index)
{
  uint64_t currentMicros  = micros();

  // Count from start to stop PWM pulse
  deltaMicrosStop[index]    = currentMicros - previousMicrosStart[index];
  
  previousMicrosStop[index] = currentMicros;
}

#endif    // #if USE_COMPLEX_STRUCT

////////////////////////////////////
// Shared
////////////////////////////////////

void doingSomethingStart0()
{
  static bool ledON = LED_OFF;
  
  doingSomethingStart(0);

  callbackTime[0]++;

  digitalWrite(LEDG, ledON);

  ledON = !ledON;
}

void doingSomethingStart1()
{
  static bool ledON = LED_OFF;
  
  doingSomethingStart(1);

  callbackTime[1]++;

  digitalWrite(LEDB, ledON);

  ledON = !ledON;
}

void doingSomethingStart2()
{
  static bool ledON = LED_OFF;

  doingSomethingStart(2);

  digitalWrite(LEDR, ledON);

  callbackTime[2]++;

  ledON = !ledON;
}

void doingSomethingStart3()
{
  doingSomethingStart(3);

  callbackTime[3]++;
}

//////////////////////////////////////////////////////

void doingSomethingStop0()
{
  doingSomethingStop(0);
}

void doingSomethingStop1()
{
  doingSomethingStop(1);
}

void doingSomethingStop2()
{
  doingSomethingStop(2);
}

void doingSomethingStop3()
{
  doingSomethingStop(3);
}


//////////////////////////////////////////////////////

#if USE_COMPLEX_STRUCT
 
  ISR_PWM_Data curISR_PWM_Data[NUMBER_ISR_PWMS] =
  {
    // pin, irqCallbackStartFunc, irqCallbackStopFunc, PWM_Freq, deltaMicrosStart, previousMicrosStart, deltaMicrosStop, previousMicrosStop
    { pin0,       doingSomethingStart0,    doingSomethingStop0,    2,  20, 0, 0, 0, 0 },
    { pin1,       doingSomethingStart1,    doingSomethingStop1,    5,  30, 0, 0, 0, 0 },
    { pin2,       doingSomethingStart2,    doingSomethingStop2,   50,  50, 0, 0, 0, 0 },
    { pin3,       doingSomethingStart3,    doingSomethingStop3,  100,  70, 0, 0, 0, 0 },
  };
  
void doingSomethingStart(int index)
{
  uint64_t currentMicros  = micros();

  curISR_PWM_Data[index].deltaMicrosStart    = currentMicros - curISR_PWM_Data[index].previousMicrosStart;
  curISR_PWM_Data[index].previousMicrosStart = currentMicros;
}

void doingSomethingStop(int index)
{
  uint64_t currentMicros  = micros();

  // Count from start to stop PWM pulse
  curISR_PWM_Data[index].deltaMicrosStop     = currentMicros - curISR_PWM_Data[index].previousMicrosStart;
  curISR_PWM_Data[index].previousMicrosStop  = currentMicros;
}

#else   // #if USE_COMPLEX_STRUCT

callback_function_t irqCallbackStartFunc[NUMBER_ISR_PWMS] =
{
  doingSomethingStart0,  doingSomethingStart1,  doingSomethingStart2,  doingSomethingStart3
};

callback_function_t irqCallbackStopFunc[NUMBER_ISR_PWMS] =
{
  doingSomethingStop0,  doingSomethingStop1,  doingSomethingStop2,  doingSomethingStop3
};

#endif    // #if USE_COMPLEX_STRUCT

//////////////////////////////////////////////////////

#define SIMPLE_TIMER_MS        2000L

// Init SimpleTimer
SimpleTimer simpleTimer;

// Here is software Timer, you can do somewhat fancy stuffs without many issues.
// But always avoid
// 1. Long delay() it just doing nothing and pain-without-gain wasting CPU power.Plan and design your code / strategy ahead
// 2. Very long "do", "while", "for" loops without predetermined exit time.
void simpleTimerDoingSomething2s()
{
  static uint64_t previousMicrosStart = startMicros;

  uint64_t currMicros = micros();

  Serial.print(F("SimpleTimer (ms): ")); Serial.print(SIMPLE_TIMER_MS);
  Serial.print(F(", us : ")); Serial.print(currMicros);
  Serial.print(F(", Dus : ")); Serial.println(currMicros - previousMicrosStart);

  for (uint16_t i = 0; i < NUMBER_ISR_PWMS; i++)
  {
#if USE_COMPLEX_STRUCT
    Serial.print(F("PWM Channel : ")); Serial.print(i);
    Serial.print(F(", programmed Period (us): "));

    Serial.print(1000000 / curISR_PWM_Data[i].PWM_Freq);
    
    Serial.print(F(", actual : ")); Serial.print(curISR_PWM_Data[i].deltaMicrosStart);
    
    Serial.print(F(", programmed DutyCycle : ")); 

    Serial.print(curISR_PWM_Data[i].PWM_DutyCycle);
    
    Serial.print(F(", actual : ")); Serial.println((float) curISR_PWM_Data[i].deltaMicrosStop * 100.0f / curISR_PWM_Data[i].deltaMicrosStart);
    
#else

    Serial.print(F("PWM Channel : ")); Serial.print(i);
    
    Serial.print(1000000 / PWM_Freq[i]);
  
    Serial.print(F(", programmed Period (us): ")); Serial.print(1000000 / PWM_Freq[i]);
    Serial.print(F(", actual : ")); Serial.print(deltaMicrosStart[i]);

    Serial.print(F(", programmed DutyCycle : ")); 
  
    Serial.print(PWM_DutyCycle[i]);
      
    Serial.print(F(", actual : ")); Serial.println( (float) deltaMicrosStop[i] * 100.0f / deltaMicrosStart[i]);
#endif
  }

  previousMicrosStart = currMicros;
}

void setup()
{
  pinMode(LEDG, OUTPUT);
  pinMode(LEDB, OUTPUT);
  pinMode(LEDR, OUTPUT);

  digitalWrite(LEDG, LED_OFF);
  digitalWrite(LEDB, LED_OFF);
  digitalWrite(LEDR, LED_OFF);

  for (uint8_t index = 0; index < NUM_OF_PINS; index++)
  {
    pinMode(pins[index], OUTPUT);
    digitalWrite(pins[index], LOW);
  }
  
  Serial.begin(115200);
  while (!Serial);

  delay(2000);

  Serial.print(F("\nStarting PWMs_Array_Complex on ")); Serial.println(BOARD_NAME);
  Serial.println(PORTENTA_H7_PWM_VERSION);
    // Automatically retrieve TIM instance and channel associated to pin
  // This is used to be compatible with all STM32 series automatically.

  startMicros = micros();
  
  for (uint8_t index = 0; index < NUM_OF_PINS; index++)
  {
    TIM_TypeDef *Instance = TimerInstance[index];

    // Automatically retrieve TIM instance and channel associated to pin
    // This is used to be compatible with all STM32 series automatically.
    //TIM_TypeDef *Instance = (TIM_TypeDef *)pinmap_peripheral(digitalPinToPinName(pins[index]), PinMap_PWM);
    
    uint32_t channel = STM_PIN_CHANNEL(pinmap_function(digitalPinToPinName(pins[index]), PinMap_PWM));

    Serial.print("Index = "); Serial.print(index);
    Serial.print(", Instance = 0x");Serial.print( (uint32_t) Instance, HEX);
    Serial.print(", channel = ");Serial.print(channel);
    Serial.print(", TimerIndex = "); Serial.println(get_timer_index(Instance));

    HardwareTimer *MyTim = new HardwareTimer(Instance);

    // void HardwareTimer::setPWM(uint32_t channel, PinName pin, uint32_t frequency, uint32_t dutycycle, 
    //                            callback_function_t PeriodCallback, callback_function_t CompareCallback)
    //MyTim->setPWM(channel, pins[index], freq[index], 50, PeriodCallback[index]);

#if USE_COMPLEX_STRUCT

    MyTim->setPWM(channel, curISR_PWM_Data[index].PWM_Pin, curISR_PWM_Data[index].PWM_Freq, curISR_PWM_Data[index].PWM_DutyCycle, 
                  curISR_PWM_Data[index].irqCallbackStartFunc, curISR_PWM_Data[index].irqCallbackStopFunc);

#else

    MyTim->setPWM(channel, PWM_Pin[index], PWM_Freq[index], PWM_DutyCycle[index], irqCallbackStartFunc[index], irqCallbackStopFunc[index]);

#endif    
  }
  
  // You need this timer for non-critical tasks. Avoid abusing ISR if not absolutely necessary.
  simpleTimer.setInterval(SIMPLE_TIMER_MS, simpleTimerDoingSomething2s);
}

#define BLOCKING_TIME_MS      10000L

void loop()
{
  // This unadvised blocking task is used to demonstrate the blocking effects onto the execution and accuracy to Software timer
  // You see the time elapse of ISR_PWM still accurate, whereas very unaccurate for Software Timer
  // The time elapse for 2000ms software timer now becomes 3000ms (BLOCKING_TIME_MS)
  // While that of ISR_PWM is still prefect.
  delay(BLOCKING_TIME_MS);

  // You need this Software timer for non-critical tasks. Avoid abusing ISR if not absolutely necessary
  // You don't need to and never call ISR_PWM.run() here in the loop(). It's already handled by ISR timer.
  simpleTimer.run();
}
