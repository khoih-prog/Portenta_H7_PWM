/****************************************************************************************************************************
  PWM_Multi.ino
  For Portenta_H7 boards
  Written by Khoi Hoang

  Built by Khoi Hoang https://github.com/khoih-prog/Portenta_H7_PWM
  Licensed under MIT license
*****************************************************************************************************************************/

#if !( defined(ARDUINO_PORTENTA_H7_M7) || defined(ARDUINO_PORTENTA_H7_M4) )
  #error For Portenta_H7 only
#endif

#define _PWM_LOGLEVEL_       1

// To be included only in main(), .ino with setup() to avoid `Multiple Definitions` Linker Error
#include "Portenta_H7_PWM.h"

#define LED_ON        LOW
#define LED_OFF       HIGH

// Can't use same TimerIndex again, e.g., the D1 and D2, using TIM1, can't be use concurrently
// That's why D0, D1, D3, D4 and D6 (using TimerIndex 8, 1, HRTIM and 3) are OK together

// Only OK for D0, D1, D2, D4 and D5, PA_0C(D15/A0), PA_1C(D16/A1),
// D3, D6, D7, D8, D9, D10, D11, D12, D13, D14, D17(PC_2C/A2), D18(PC_3C/3), PC2(D19/A4) LEDG, LEDB not OK
#define pinD0    D0       // PH15 / TIM8_CH3N
#define pinD1    D1       // PK1  / TIM1_CH1, TIM8_CH3
#define pinD2    D2       // PJ11 / TIM1_CH2, TIM8_CH2N
#define pinD3    D3       // PG7  / HRTIM_CHE2
#define pinD4    D4       // PC7  / TIM3_CH2, TIM8_CH2, HRTIM_CHA2
#define pinD5    D5       // PC6  / TIM3_CH1, TIM8_CH1, HRTIM_CHA1, LPTIM3_OUT
#define pinD6    D6       // PA8  / HRTIM_CHB2 (TIM1_CH1, TIM8_BKIN2)

// See https://www.st.com/resource/en/datasheet/stm32h747xi.pdf, Table 7, page 53
// Can't use pins with same TIMx. For example,
// pinD1 and pinD2, using same TIM1, can't be used at the same time
// pinD4 and pinD5, using same TIM3, can't be used at the same time
// pinD3 and pinD6 are using HRTIM, can't be used at the same time and the minimum freq must be ~770Hz
uint32_t pins[]       = { pinD0, pinD1, pinD3, pinD5 };

#define NUM_OF_PINS       ( sizeof(pins) / sizeof(uint32_t) )

float dutyCycle[]      = { 50.0f, 50.0f, 50.0f, 50.0f };

float freq[]           = { 1000.0f, 2500.0f, 4000.0f, 5000.0f };

float curDutyCycle[]   = { 50.0f, 50.0f, 50.0f, 50.0f };

float curFreq[]        = { 1000.0f, 2500.0f, 4000.0f, 5000.0f };

mbed::PwmOut* pwm[]    = { NULL, NULL, NULL, NULL };

void startAllPWM()
{
  digitalWrite(LEDG, LED_ON);
  digitalWrite(LEDB, LED_OFF);
  digitalWrite(LEDR, LED_OFF);

  for (uint8_t index = 0; index < NUM_OF_PINS; index++)
  {
    PWM_LOGERROR7("Freq = ", freq[index], ", \tDutyCycle % = ", dutyCycle[index], ", \tDutyCycle = ",
                  dutyCycle[index] / 100, ", \tPin = ", pins[index]);

    // setPWM(mbed::PwmOut* &pwm, pin_size_t pin, float frequency, float dutyCycle)
    setPWM(pwm[index], pins[index], freq[index], dutyCycle[index]);
  }
}

void restoreAllPWM()
{
  digitalWrite(LEDG, LED_ON);
  digitalWrite(LEDB, LED_OFF);
  digitalWrite(LEDR, LED_OFF);

  for (uint8_t index = 0; index < NUM_OF_PINS; index++)
  {
    curFreq[index]      = freq[index];
    curDutyCycle[index] = dutyCycle[index];

    // setPWM(mbed::PwmOut* &pwm, pin_size_t pin, float frequency, float dutyCycle)
    setPWM(pwm[index], pins[index], freq[index], dutyCycle[index]);
  }
}

void changeAllPWM()
{
  digitalWrite(LEDG, LED_OFF);
  digitalWrite(LEDB, LED_ON);
  digitalWrite(LEDR, LED_OFF);

  for (uint8_t index = 0; index < NUM_OF_PINS; index++)
  {
    curFreq[index]      = freq[index] * 2;
    curDutyCycle[index] = dutyCycle[index] / 2;

    // setPWM(mbed::PwmOut* &pwm, pin_size_t pin, float frequency, float dutyCycle)
    setPWM(pwm[index], pins[index], curFreq[index], curDutyCycle[index]);
  }
}

void stopAllPWM()
{
  digitalWrite(LEDG, LED_OFF);
  digitalWrite(LEDB, LED_OFF);
  digitalWrite(LEDR, LED_ON);

  for (uint8_t index = 0; index < NUM_OF_PINS; index++)
  {
    curFreq[index]      = 1000.0f;
    curDutyCycle[index] = 0.0f;

    //stopPWM(mbed::PwmOut* &pwm, pin_size_t pin)
    stopPWM(pwm[index], pins[index]);
  }
}

void printLine()
{
  Serial.println(
    F("\n=========================================================================================================="));
}

void printPulseWidth()
{
  static uint32_t num = 0;

  if (num++ % 50 == 0)
  {
    printLine();

    for (uint8_t index = 0; index < NUM_OF_PINS; index++)
    {
      Serial.print(F("PW (us) "));
      Serial.print(index);
      Serial.print(F("\t"));
    }

    printLine();
  }

  if (num > 1)
  {
    for (uint8_t index = 0; index < NUM_OF_PINS; index++)
    {
      if (pwm[index])
      {
        if ( (pins[index] == pinD3) || (pins[index] == pinD6) )
        {
          // Using HRTIM => fake by calculating PW
          Serial.print( (10000 * curDutyCycle[index]) / curFreq[index] );
          Serial.print(F("\t\t"));
        }
        else
        {
          Serial.print((float) pwm[index]->read_pulsewitdth_us());
          Serial.print(F("\t\t"));
        }
      }
    }

    Serial.println();
  }
}

#define PRINT_INTERVAL      10000L
#define CHANGE_INTERVAL     20000L

void check_status()
{
  static unsigned long checkstatus_timeout  = 0;
  static unsigned long changePWM_timeout    = 0;

  static bool PWM_orig  = true;
  static uint32_t count = 0;

  // Print every PRINT_INTERVAL (10) seconds.
  if ((millis() > checkstatus_timeout) || (checkstatus_timeout == 0))
  {
    printPulseWidth();
    checkstatus_timeout = millis() + PRINT_INTERVAL;
  }

  if ( (millis() > changePWM_timeout) && (millis() > CHANGE_INTERVAL) )
  {

    if (PWM_orig)
    {
      if (count++ % 2 == 0)
      {
        Serial.println("Stop all PWM");
        stopAllPWM();
      }
      else
      {
        Serial.println("Change all PWM");

        changeAllPWM();

        PWM_orig = !PWM_orig;
      }
    }
    else
    {
      Serial.println("Restore all PWM");

      restoreAllPWM();

      PWM_orig = !PWM_orig;
    }

    changePWM_timeout = millis() + CHANGE_INTERVAL;
  }
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

  delay(100);

  Serial.print(F("\nStarting PWM_Multi on "));
  Serial.println(BOARD_NAME);
  Serial.println(PORTENTA_H7_PWM_VERSION);

  // Automatically retrieve TIM instance and channel associated to pin
  // This is used to be compatible with all STM32 series automatically.

  startAllPWM();
}

void loop()
{
  check_status();
}
