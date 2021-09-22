# Portenta_H7_PWM Library

[![arduino-library-badge](https://www.ardu-badge.com/badge/Portenta_H7_PWM.svg?)](https://www.ardu-badge.com/Portenta_H7_PWM)
[![GitHub release](https://img.shields.io/github/release/khoih-prog/Portenta_H7_PWM.svg)](https://github.com/khoih-prog/Portenta_H7_PWM/releases)
[![GitHub](https://img.shields.io/github/license/mashape/apistatus.svg)](https://github.com/khoih-prog/Portenta_H7_PWM/blob/master/LICENSE)
[![contributions welcome](https://img.shields.io/badge/contributions-welcome-brightgreen.svg?style=flat)](#Contributing)
[![GitHub issues](https://img.shields.io/github/issues/khoih-prog/Portenta_H7_PWM.svg)](http://github.com/khoih-prog/Portenta_H7_PWM/issues)

---
---

## Table of Contents

* [Why do we need this Portenta_H7_PWM library](#why-do-we-need-this-Portenta_H7_PWM-library)
  * [Features](#features)
  * [Why using ISR-based Hardware Timer Interrupt is better](#why-using-isr-based-hardware-timer-interrupt-is-better)
  * [Currently supported Boards](#currently-supported-boards)
  * [Important Notes about ISR](#important-notes-about-isr)
* [Prerequisites](#prerequisites)
* [Installation](#installation)
  * [Use Arduino Library Manager](#use-arduino-library-manager)
  * [Manual Install](#manual-install)
  * [VS Code & PlatformIO](#vs-code--platformio)
* [Packages' Patches](#packages-patches)
  * [1. For Portenta_H7 boards using Arduino IDE in Linux](#1-for-portenta_h7-boards-using-arduino-ide-in-linux)
* [More useful Information about STM32 Timers](#more-useful-information-about-stm32-timers)
* [Available Timers for Portenta_H7](#available-timers-for-portenta_h7)
* [Usage](#usage)
  * [1. Using only Hardware Timer directly](#1-using-only-hardware-timer-directly)
    * [1.1 Init Hardware Timer](#11-init-hardware-timer)
    * [1.2 Set PWM Frequency, dutycycle and attach PeriodCallback function](#12-Set-PWM-Frequency-dutycycle-and-attach-PeriodCallback-function)
* [Examples](#examples)
  * [ 1. PWM_Multi](examples/PWM_Multi)
  * [ 2. PWM_Multi_Args](examples/PWM_Multi_Args)
  * [ 3. PWMs_Array_Complex](examples/PWMs_Array_Complex)
* [Example PWM_Multi](#example-PWM_Multi)
* [Debug Terminal Output Samples](#debug-terminal-output-samples)
  * [1. PWMs_Array_Complex on PORTENTA_H7_M7](#1-PWMs_Array_Complex-on-PORTENTA_H7_M7)
  * [2. PWM_Multi_Args on PORTENTA_H7_M7](#2-PWM_Multi_Args-on-PORTENTA_H7_M7)
  * [3. PWM_Multi on PORTENTA_H7_M7](#3-PWM_Multi-on-PORTENTA_H7_M7)
* [Debug](#debug)
* [Troubleshooting](#troubleshooting)
* [Issues](#issues)
* [TO DO](#to-do)
* [DONE](#done)
* [Contributions and Thanks](#contributions-and-thanks)
* [Contributing](#contributing)
* [License](#license)
* [Copyright](#copyright)

---
---

### Why do we need this [Portenta_H7_PWM library](https://github.com/khoih-prog/Portenta_H7_PWM)

### Features

This library enables you to use Hardware Timers on an STM32H7-based Portenta_H7 board to create and output PWM to pins. 

---

The most important feature is they're Ipurely hardware-based PWM channels. Therefore, their executions are **not blocked by bad-behaving functions / tasks**. This important feature is absolutely necessary for mission-critical tasks. 

This important feature is absolutely necessary for mission-critical tasks. These hardware timers, using interrupt, still work even if other functions are blocking. Moreover, they are much more precise (certainly depending on clock frequency accuracy) than other software timers using millis() or micros(). That's necessary if you need to measure some data requiring better accuracy.

The [**PWMs_Array_Complex**](examples/PWMs_Array_Complex) example will demonstrate the nearly perfect accuracy, compared to software timers, by printing the actual period / duty-cycle in `microsecs` of each of PWM-channels.

The [**PWM_Multi_Args**](examples/PWM/PWM_Multi_Args) will demonstrate the usage of multichannel PWM using multiple Hardware Timers. The 4 independent Hardware Timers are used **to control 4 different PWM outputs**, with totally independent frequencies and dutycycles.

Being ISR-based PWM, their executions are not blocked by bad-behaving functions / tasks, such as connecting to WiFi, Internet or Blynk services. You can also have many `(up to 16)` timers to use.

This non-being-blocked important feature is absolutely necessary for mission-critical tasks.

You'll see `software-based` SimpleTimer is blocked while system is connecting to WiFi / Internet / Blynk, as well as by blocking task 
in loop(), using delay() function as an example. The elapsed time then is very unaccurate

---

#### Why using ISR-based PWM is better

Imagine you have a system with a **mission-critical** function, measuring water level and control the sump pump or doing something much more important. You normally use a software timer to poll, or even place the function in loop(). But what if another function is **blocking** the loop() or setup().

So your function **might not be executed, and the result would be disastrous.**

You'd prefer to have your function called, no matter what happening with other functions (busy loop, bug, etc.).

The correct choice is to use a Hardware Timer with **Interrupt** to call your function.

These hardware timers, using interrupt, still work even if other functions are blocking. Moreover, they are much more **precise** (certainly depending on clock frequency accuracy) than other software timers using millis() or micros(). That's necessary if you need to measure some data requiring better accuracy.

Functions using normal software timers, relying on loop() and calling millis(), won't work if the loop() or setup() is blocked by certain operation. For example, certain function is blocking while it's connecting to WiFi or some services.

The catch is **your function is now part of an ISR (Interrupt Service Routine), and must be lean / mean, and follow certain rules.** More to read on:

[**HOWTO Attach Interrupt**](https://www.arduino.cc/reference/en/language/functions/external-interrupts/attachinterrupt/)

---

### Currently supported Boards

1. **Portenta_H7 boards** such as Portenta_H7 Rev2 ABX00042, etc., using [**ArduinoCore-mbed mbed_portenta** core](https://github.com/arduino/ArduinoCore-mbed)

---

#### Important Notes about ISR

1. Inside the attached function, **delay() won’t work and the value returned by millis() will not increment.** Serial data received while in the function may be lost. You should declare as **volatile any variables that you modify within the attached function.**

2. Typically global variables are used to pass data between an ISR and the main program. To make sure variables shared between an ISR and the main program are updated correctly, declare them as volatile.

---
---

## Prerequisites

 1. [`Arduino IDE 1.8.16+` for Arduino](https://www.arduino.cc/en/Main/Software)
 2. [`ArduinoCore-mbed mbed_portenta core 2.4.1+`](https://github.com/arduino/ArduinoCore-mbed) for Arduino **Portenta_H7** boards, such as **Portenta_H7 Rev2 ABX00042, etc.**. [![GitHub release](https://img.shields.io/github/release/arduino/ArduinoCore-mbed.svg)](https://github.com/arduino/ArduinoCore-mbed/releases/latest)

 3. To use with certain example
   - [`SimpleTimer library`](https://github.com/jfturcot/SimpleTimer) for [ISR_16_Timers_Array example](examples/ISR_16_Timers_Array).
---
---

## Installation

### Use Arduino Library Manager

The best and easiest way is to use `Arduino Library Manager`. Search for [**Portenta_H7_PWM**](https://github.com/khoih-prog/Portenta_H7_PWM), then select / install the latest version.
You can also use this link [![arduino-library-badge](https://www.ardu-badge.com/badge/Portenta_H7_PWM.svg?)](https://www.ardu-badge.com/Portenta_H7_PWM) for more detailed instructions.

### Manual Install

Another way to install is to:

1. Navigate to [**Portenta_H7_PWM**](https://github.com/khoih-prog/Portenta_H7_PWM) page.
2. Download the latest release `Portenta_H7_PWM-master.zip`.
3. Extract the zip file to `Portenta_H7_PWM-master` directory 
4. Copy whole `Portenta_H7_PWM-master` folder to Arduino libraries' directory such as `~/Arduino/libraries/`.

### VS Code & PlatformIO

1. Install [VS Code](https://code.visualstudio.com/)
2. Install [PlatformIO](https://platformio.org/platformio-ide)
3. Install [**Portenta_H7_PWM** library](https://platformio.org/lib/show/xxxxx/Portenta_H7_PWM) by using [Library Manager](https://platformio.org/lib/show/xxxxx/Portenta_H7_PWM/installation). Search for **Portenta_H7_PWM** in [Platform.io Author's Libraries](https://platformio.org/lib/search?query=author:%22Khoi%20Hoang%22)
4. Use included [platformio.ini](platformio/platformio.ini) file from examples to ensure that all dependent libraries will installed automatically. Please visit documentation for the other options and examples at [Project Configuration File](https://docs.platformio.org/page/projectconf.html)

---
---

### Packages' Patches

#### 1. For Portenta_H7 boards using Arduino IDE in Linux

  **To be able to upload firmware to Portenta_H7 using Arduino IDE in Linux (Ubuntu, etc.)**, you have to copy the file [portenta_post_install.sh](Packages_Patches/arduino/hardware/mbed_portenta/2.4.1/portenta_post_install.sh) into mbed_portenta directory (~/.arduino15/packages/arduino/hardware/mbed_portenta/2.4.1/portenta_post_install.sh). 
  
  Then run the following command using `sudo`
  
```
$ cd ~/.arduino15/packages/arduino/hardware/mbed_portenta/2.4.1
$ chmod 755 portenta_post_install.sh
$ sudo ./portenta_post_install.sh
```

This will create the file `/etc/udev/rules.d/49-portenta_h7.rules` as follows:

```
# Portenta H7 bootloader mode UDEV rules

SUBSYSTEMS=="usb", ATTRS{idVendor}=="2341", ATTRS{idProduct}=="035b", GROUP="plugdev", MODE="0666"
```

Supposing the ArduinoCore-mbed core version is 2.4.1. Now only one file must be copied into the directory:

- `~/.arduino15/packages/arduino/hardware/mbed_portenta/2.4.1/portenta_post_install.sh`

Whenever a new version is installed, remember to copy this files into the new version directory. For example, new version is x.yy.zz

This file must be copied into the directory:

- `~/.arduino15/packages/arduino/hardware/mbed_portenta/x.yy.zz/portenta_post_install.sh`


---
---


## More useful Information about STM32 Timers

The Timers of STM32s are numerous, yet very sophisticated and powerful.

In general, across the STM32 microcontrollers families, the timer peripherals that have the same name also have the same features set, but there are a few exceptions. 

The general purpose timers embedded by the STM32 microcontrollers share the same backbone structure; they differ only on the level of features embedded by a given timer peripheral. 

The level of features integration for a given timer peripheral is decided based on the applications field that it targets.

The timer peripherals can be classified as:

• Advanced-configuration timers like TIM1 and TIM8 among others.
• General-purpose configuration timers like TIM2 and TIM3 among others
• Lite-configuration timers like TIM9, TIM10, TIM12 and TIM16 among others
• Basic-configuration timers like TIM6 and TIM7 among others.


More information can be found at [**Embedded-Lab STM32 TIMERS**](http://embedded-lab.com/blog/stm32-timers/)

To be sure which Timer is available for the board you're using, check the Core Package's related files. For example, for **Portenta_H7 using STM32H747XI**, check this file:

1. `~/.arduino15/packages/STM32/hardware/stm32/2.0.0/system/Drivers/CMSIS/Device/ST/STM32H7xx/Include/stm32h7xx.h`


The information will be as follows:

```
typedef struct
{
  __IO uint32_t CR1;         /*!< TIM control register 1,                   Address offset: 0x00 */
  __IO uint32_t CR2;         /*!< TIM control register 2,                   Address offset: 0x04 */
  __IO uint32_t SMCR;        /*!< TIM slave mode control register,          Address offset: 0x08 */
  __IO uint32_t DIER;        /*!< TIM DMA/interrupt enable register,        Address offset: 0x0C */
  __IO uint32_t SR;          /*!< TIM status register,                      Address offset: 0x10 */
  __IO uint32_t EGR;         /*!< TIM event generation register,            Address offset: 0x14 */
  __IO uint32_t CCMR1;       /*!< TIM capture/compare mode register 1,      Address offset: 0x18 */
  __IO uint32_t CCMR2;       /*!< TIM capture/compare mode register 2,      Address offset: 0x1C */
  __IO uint32_t CCER;        /*!< TIM capture/compare enable register,      Address offset: 0x20 */
  __IO uint32_t CNT;         /*!< TIM counter register,                     Address offset: 0x24 */
  __IO uint32_t PSC;         /*!< TIM prescaler,                            Address offset: 0x28 */
  __IO uint32_t ARR;         /*!< TIM auto-reload register,                 Address offset: 0x2C */
  __IO uint32_t RCR;         /*!< TIM repetition counter register,          Address offset: 0x30 */
  __IO uint32_t CCR1;        /*!< TIM capture/compare register 1,           Address offset: 0x34 */
  __IO uint32_t CCR2;        /*!< TIM capture/compare register 2,           Address offset: 0x38 */
  __IO uint32_t CCR3;        /*!< TIM capture/compare register 3,           Address offset: 0x3C */
  __IO uint32_t CCR4;        /*!< TIM capture/compare register 4,           Address offset: 0x40 */
  __IO uint32_t BDTR;        /*!< TIM break and dead-time register,         Address offset: 0x44 */
  __IO uint32_t DCR;         /*!< TIM DMA control register,                 Address offset: 0x48 */
  __IO uint32_t DMAR;        /*!< TIM DMA address for full transfer,        Address offset: 0x4C */
  uint32_t      RESERVED1;   /*!< Reserved, 0x50                                                 */
  __IO uint32_t CCMR3;       /*!< TIM capture/compare mode register 3,      Address offset: 0x54 */
  __IO uint32_t CCR5;        /*!< TIM capture/compare register5,            Address offset: 0x58 */
  __IO uint32_t CCR6;        /*!< TIM capture/compare register6,            Address offset: 0x5C */
  __IO uint32_t AF1;         /*!< TIM alternate function option register 1, Address offset: 0x60 */
  __IO uint32_t AF2;         /*!< TIM alternate function option register 2, Address offset: 0x64 */
  __IO uint32_t TISEL;       /*!< TIM Input Selection register,             Address offset: 0x68 */
} TIM_TypeDef;
```

and

```
#define PERIPH_BASE            0x40000000UL /*!< Base address of : AHB/ABP Peripherals   
/*!< Peripheral memory map */
#define APB1PERIPH_BASE        PERIPH_BASE

/*!< APB1 peripherals */
/*!< D2_APB1PERIPH peripherals */
#define TIM2_BASE             (D2_APB1PERIPH_BASE + 0x0000UL)
#define TIM3_BASE             (D2_APB1PERIPH_BASE + 0x0400UL)
#define TIM4_BASE             (D2_APB1PERIPH_BASE + 0x0800UL)
#define TIM5_BASE             (D2_APB1PERIPH_BASE + 0x0C00UL)
#define TIM6_BASE             (D2_APB1PERIPH_BASE + 0x1000UL)
#define TIM7_BASE             (D2_APB1PERIPH_BASE + 0x1400UL)
#define TIM12_BASE            (D2_APB1PERIPH_BASE + 0x1800UL)
#define TIM13_BASE            (D2_APB1PERIPH_BASE + 0x1C00UL)
#define TIM14_BASE            (D2_APB1PERIPH_BASE + 0x2000UL)

/*!< APB2 peripherals */
#define TIM1_BASE             (D2_APB2PERIPH_BASE + 0x0000UL)
#define TIM8_BASE             (D2_APB2PERIPH_BASE + 0x0400UL)
...
#define TIM9_BASE             (APB2PERIPH_BASE + 0x4000UL)
#define TIM10_BASE            (APB2PERIPH_BASE + 0x4400UL)
#define TIM11_BASE            (APB2PERIPH_BASE + 0x4800UL)
...
#define TI15_BASE            (D2_APB2PERIPH_BASE + 0x4000UL)
#define TIM16_BASE            (D2_APB2PERIPH_BASE + 0x4400UL)
#define TIM17_BASE            (D2_APB2PERIPH_BASE + 0x4800UL)
...
#define HRTIM1_BASE           (D2_APB2PERIPH_BASE + 0x7400UL)
#define HRTIM1_TIMA_BASE      (HRTIM1_BASE + 0x00000080UL)
#define HRTIM1_TIMB_BASE      (HRTIM1_BASE + 0x00000100UL)
#define HRTIM1_TIMC_BASE      (HRTIM1_BASE + 0x00000180UL)
#define HRTIM1_TIMD_BASE      (HRTIM1_BASE + 0x00000200UL)
#define HRTIM1_TIME_BASE      (HRTIM1_BASE + 0x00000280UL)
#define HRTIM1_COMMON_BASE    (HRTIM1_BASE + 0x00000380UL)
...
#define TIM2                ((TIM_TypeDef *) TIM2_BASE)
#define TIM3                ((TIM_TypeDef *) TIM3_BASE)
#define TIM4                ((TIM_TypeDef *) TIM4_BASE)
#define TIM5                ((TIM_TypeDef *) TIM5_BASE)
#define TIM6                ((TIM_TypeDef *) TIM6_BASE)
#define TIM7                ((TIM_TypeDef *) TIM7_BASE)
#define TIM13               ((TIM_TypeDef *) TIM13_BASE)
#define TIM14               ((TIM_TypeDef *) TIM14_BASE)
...
#define TIM1                ((TIM_TypeDef *) TIM1_BASE)
#define TIM8                ((TIM_TypeDef *) TIM8_BASE)
...
#define TIM12               ((TIM_TypeDef *) TIM12_BASE)
#define TIM15               ((TIM_TypeDef *) TIM15_BASE)
#define TIM16               ((TIM_TypeDef *) TIM16_BASE)
#define TIM17               ((TIM_TypeDef *) TIM17_BASE)
...
#define HRTIM1              ((HRTIM_TypeDef *) HRTIM1_BASE)
#define HRTIM1_TIMA         ((HRTIM_Timerx_TypeDef *) HRTIM1_TIMA_BASE)
#define HRTIM1_TIMB         ((HRTIM_Timerx_TypeDef *) HRTIM1_TIMB_BASE)
#define HRTIM1_TIMC         ((HRTIM_Timerx_TypeDef *) HRTIM1_TIMC_BASE)
#define HRTIM1_TIMD         ((HRTIM_Timerx_TypeDef *) HRTIM1_TIMD_BASE)
#define HRTIM1_TIME         ((HRTIM_Timerx_TypeDef *) HRTIM1_TIME_BASE)
#define HRTIM1_COMMON       ((HRTIM_Common_TypeDef *) HRTIM1_COMMON_BASE)
```

---

## Available Timers for Portenta_H7

This is the temporary list for Portenta_H7 Timers which can be used. The available Timers certainly depends on they are being used for other purpose (core, application, libraries, etc.) or not. You have to exhausively test yourself to be sure.

#### 1. OK to use

**TIM1, TIM4, TIM7, TIM8, TIM12, TIM13, TIM14, TIM15, TIM16, TIM17**

#### 2. Not exist

**TIM9, TIM10, TIM11. Only for STM32F2, STM32F4 or STM32L1**

#### 3.Not declared

**TIM18, TIM19, TIM20, TIM21, TIM22**

#### 3. Not OK => conflict or crash

**TIM2, TIM3, TIM5, TIM6**


---
---

## Usage

Before using any Timer for a PWM channel, you have to make sure the Timer has not been used by any other purpose.

### 1. Using only Hardware Timer directly

#### 1.1 Init Hardware Timer

```
HardwareTimer *MyTim = new HardwareTimer(TIM8);
```

#### 1.2 Set PWM Frequency, dutycycle and attach PeriodCallback function

```
void PeriodCallback()
{

}

void setup()
{
  ....
  
  MyTim->setPWM(channel, pins, freq, dutyCycle, PeriodCallback);
}  
```

---
---

### Examples: 

 1. [PWM_Multi](examples/PWM_Multi)
 2. [PWM_Multi_Args](examples/PWM_Multi_Args)
 3. [PWMs_Array_Complex](examples/PWMs_Array_Complex)

 
---
---

### Example [PWM_Multi](examples/PWM_Multi)

```
#if !( defined(ARDUINO_PORTENTA_H7_M7) || defined(ARDUINO_PORTENTA_H7_M4) )
  #error For Portenta_H7 only
#endif

#include "Portenta_H7_PWM.h"

#define LED_ON        LOW
#define LED_OFF       HIGH

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

uint32_t pins[]       = { pin0, pin1, pin2, pin3, pin4, pin5, pin6 };

#define NUM_OF_PINS       ( sizeof(pins) / sizeof(uint32_t) )

uint32_t dutyCycle[NUM_OF_PINS]  = { 10, 20, 30, 50, 70, 90, 100 };

uint32_t freq[]       = { 1, 2, 5, 10, 20, 50, 100 };

//TIM_TypeDef *TimerInstance[] = { TIM8, TIM12, TIM13, TIM14, TIM15, TIM16, TIM17 };
TIM_TypeDef *TimerInstance[] = { TIM1, TIM4, TIM7, TIM8, TIM12, TIM13, TIM14 };

volatile uint32_t callbackTime[] = { 0, 0, 0, 0, 0, 0, 0 };

//callback_function_t PeriodCallback0()
void PeriodCallback0()
{
  static bool ledON = LED_OFF;

  callbackTime[0]++;

  digitalWrite(LEDG, ledON);

  ledON = !ledON;
}

void PeriodCallback1()
{
  static bool ledON = LED_OFF;

  digitalWrite(LEDB, ledON);

  callbackTime[1]++;

  ledON = !ledON;
}

void PeriodCallback2()
{
  static bool ledON = LED_OFF;

  digitalWrite(LEDR, ledON);

  callbackTime[2]++;

  ledON = !ledON;
}

void PeriodCallback3()
{
  callbackTime[3]++;
}

void PeriodCallback4()
{
  callbackTime[4]++;
}

void PeriodCallback5()
{
  callbackTime[5]++;
}

void PeriodCallback6()
{
  callbackTime[6]++;
}

callback_function_t PeriodCallback[] =
{
  PeriodCallback0,  PeriodCallback1,  PeriodCallback2,  PeriodCallback3, PeriodCallback4, PeriodCallback5, PeriodCallback6
};


void printLine()
{
  Serial.println(F("\n=========================================================================================================="));
}

void printCount()
{
  static uint32_t num = 0;

  if (num++ % 50 == 0)
  {
    printLine();
    
    for (uint8_t index = 0; index < NUM_OF_PINS; index++)
    {
      Serial.print(F("Count ")); Serial.print(index); Serial.print(F("\t\t"));  
    }

    printLine();
  }
 
  if (num > 1)
  {
    for (uint8_t index = 0; index < NUM_OF_PINS; index++)
    {
      Serial.print(callbackTime[index]); Serial.print(F("\t\t"));  
    }

    Serial.println();
  }
}

#define PRINT_INTERVAL    10000L

void check_status()
{
  static unsigned long checkstatus_timeout = 0;

  // Print every PRINT_INTERVAL (10) seconds.
  if ((millis() > checkstatus_timeout) || (checkstatus_timeout == 0))
  {
    printCount();
    checkstatus_timeout = millis() + PRINT_INTERVAL;
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

  Serial.print(F("\nStarting PWM_Multi on ")); Serial.println(BOARD_NAME);
  Serial.println(PORTENTA_H7_PWM_VERSION);

  // Automatically retrieve TIM instance and channel associated to pin
  // This is used to be compatible with all STM32 series automatically.

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
    MyTim->setPWM(channel, pins[index], freq[index], 50, PeriodCallback[index]);
  }
}

void loop()
{
  check_status();
}
```
---
---

### Debug Terminal Output Samples

### 1. PWMs_Array_Complex on PORTENTA_H7_M7

The following is the sample terminal output when running example [PWMs_Array_Complex](examples/PWMs_Array_Complex) on **Portenta_H7** to demonstrate the accuracy of Hardware-based PWM, **especially when system is very busy**.


```
Starting PWMs_Array_Complex on PORTENTA_H7_M7
Portenta_H7_PWM v1.0.0
Index = 0, Instance = 0x40010000, channel = 3, TimerIndex = 0
Index = 1, Instance = 0x40000800, channel = 1, TimerIndex = 3
Index = 2, Instance = 0x40010400, channel = 2, TimerIndex = 7
Index = 3, Instance = 0x40001800, channel = 2, TimerIndex = 8
SimpleTimer (ms): 2000, us : 12659505, Dus : 9933701
PWM Channel : 0, programmed Period (us): 500000, actual : 499994, programmed DutyCycle : 20, actual : 20.00
PWM Channel : 1, programmed Period (us): 200000, actual : 199999, programmed DutyCycle : 30, actual : 30.00
PWM Channel : 2, programmed Period (us): 100000, actual : 100000, programmed DutyCycle : 50, actual : 50.00
PWM Channel : 3, programmed Period (us): 10000, actual : 10000, programmed DutyCycle : 70, actual : 70.00
SimpleTimer (ms): 2000, us : 22594086, Dus : 9934581
PWM Channel : 0, programmed Period (us): 500000, actual : 499994, programmed DutyCycle : 20, actual : 20.00
PWM Channel : 1, programmed Period (us): 200000, actual : 199998, programmed DutyCycle : 30, actual : 30.00
PWM Channel : 2, programmed Period (us): 100000, actual : 99999, programmed DutyCycle : 50, actual : 50.00
PWM Channel : 3, programmed Period (us): 10000, actual : 10000, programmed DutyCycle : 70, actual : 70.00
SimpleTimer (ms): 2000, us : 32528951, Dus : 9934865
PWM Channel : 0, programmed Period (us): 500000, actual : 499994, programmed DutyCycle : 20, actual : 20.00
PWM Channel : 1, programmed Period (us): 200000, actual : 199999, programmed DutyCycle : 30, actual : 30.00
PWM Channel : 2, programmed Period (us): 100000, actual : 100000, programmed DutyCycle : 50, actual : 50.00
PWM Channel : 3, programmed Period (us): 10000, actual : 10001, programmed DutyCycle : 70, actual : 69.99
SimpleTimer (ms): 2000, us : 42463635, Dus : 9934684
PWM Channel : 0, programmed Period (us): 500000, actual : 499994, programmed DutyCycle : 20, actual : 20.00
PWM Channel : 1, programmed Period (us): 200000, actual : 199999, programmed DutyCycle : 30, actual : 30.00
PWM Channel : 2, programmed Period (us): 100000, actual : 99999, programmed DutyCycle : 50, actual : 50.00
PWM Channel : 3, programmed Period (us): 10000, actual : 10000, programmed DutyCycle : 70, actual : 69.99
SimpleTimer (ms): 2000, us : 52398502, Dus : 9934867
PWM Channel : 0, programmed Period (us): 500000, actual : 499994, programmed DutyCycle : 20, actual : 20.00
PWM Channel : 1, programmed Period (us): 200000, actual : 199998, programmed DutyCycle : 30, actual : 30.00
PWM Channel : 2, programmed Period (us): 100000, actual : 99999, programmed DutyCycle : 50, actual : 50.00
PWM Channel : 3, programmed Period (us): 10000, actual : 10000, programmed DutyCycle : 70, actual : 70.00
```

---

### 2. PWM_Multi_Args on PORTENTA_H7_M7

The following is the sample terminal output when running example [**PWM_Multi_Args**](examples/PWM_Multi_Args) on **Portenta_H7** to demonstrate how to use multiple PWM channels with callback functions with arguments.

```
Starting PWM_Multi_Args on PORTENTA_H7_M7
Portenta_H7_PWM v1.0.0
Index = 0, Instance = 0x40010000, channel = 3, TimerIndex = 0
Index = 1, Instance = 0x40000800, channel = 1, TimerIndex = 3
Index = 2, Instance = 0x40001400, channel = 2, TimerIndex = 6
Index = 3, Instance = 0x40010400, channel = 2, TimerIndex = 7
Index = 4, Instance = 0x40001800, channel = 1, TimerIndex = 8
Index = 5, Instance = 0x40001C00, channel = 3, TimerIndex = 9
Index = 6, Instance = 0x40002000, channel = 1, TimerIndex = 10

==========================================================================================================
Count 0		Count 1		Count 2		Count 3		Count 4		Count 5		Count 6		
==========================================================================================================
10		20		50		100		199		497		993		
20		40		100		199		397		993		1985		
30		60		149		298		596		1489		2978		
40		80		199		398		795		1986		3971		
50		100		249		497		993		2482		4964		
60		120		298		596		1192		2979		5957		
70		139		348		695		1390		3475		6950		
80		159		398		795		1589		3972		7943		
90		179		447		894		1788		4469		8937		
100		199		497		993		1986		4965		9930		
110		219		547		1093		2185		5462		10923		
120		239		596		1192		2384		5959		11917		
130		259		646		1292		2583		6456		12910		
140		279		696		1391		2781		6952		13904		
149		298		745		1490		2980		7449		14898		
159		318		795		1590		3179		7946		15891		
169		338		845		1689		3377		8443		16885		
179		358		894		1788		3576		8940		17879		
189		378		944		1888		3775		9437		18873		
199		398		994		1987		3974		9934		19867		
209		418		1044		2087		4173		10431		20861		
219		438		1093		2186		4371		10928		21855		
```

---

### 3. PWM_Multi on PORTENTA_H7_M7

The following is the sample terminal output when running example [**PWM_Multi**](examples/PWM_Multi) on **Portenta_H7** to demonstrate how to use multiple PWM channels.

```
Starting PWM_Multi on PORTENTA_H7_M7
Portenta_H7_PWM v1.0.0
Index = 0, Instance = 0x40010000, channel = 3, TimerIndex = 0
Index = 1, Instance = 0x40000800, channel = 1, TimerIndex = 3
Index = 2, Instance = 0x40001400, channel = 2, TimerIndex = 6
Index = 3, Instance = 0x40010400, channel = 2, TimerIndex = 7
Index = 4, Instance = 0x40001800, channel = 1, TimerIndex = 8
Index = 5, Instance = 0x40001C00, channel = 3, TimerIndex = 9
Index = 6, Instance = 0x40002000, channel = 1, TimerIndex = 10

==========================================================================================================
Count 0		Count 1		Count 2		Count 3		Count 4		Count 5		Count 6		
==========================================================================================================
10		20		50		100		199		498		995		
20		40		100		199		398		995		1989		
30		60		150		299		597		1492		2983		
40		80		199		398		796		1989		3977		
50		100		249		498		995		2486		4972		
60		120		299		597		1194		2983		5966		
70		140		348		696		1392		3480		6960		
80		160		398		796		1591		3978		7955		
90		179		448		895		1790		4475		8949		
100		199		498		995		1989		4972		9943		
110		219		547		1094		2188		5469		10938		
120		239		597		1194		2387		5966		11932		
130		259		647		1293		2586		6464		12927		
140		279		697		1393		2785		6961		13921
```


---
---

### Debug

Debug is enabled by default on Serial.

You can also change the debugging level `_PWM_LOGLEVEL_` from 0 to 4

```cpp
// Don't define _PWM_LOGLEVEL_ > 0. Only for special ISR debugging only. Can hang the system.
#define _PWM_LOGLEVEL_     0
```

---

### Troubleshooting

If you get compilation errors, more often than not, you may need to install a newer version of the core for Arduino boards.

Sometimes, the library will only work if you update the board core to the latest version because I am using newly added functions.


---
---

### Issues

Submit issues to: [Portenta_H7_PWM issues](https://github.com/khoih-prog/Portenta_H7_PWM/issues)

---

## TO DO

1. Search for bug and improvement.
2. Similar features for remaining Arduino boards

---

## DONE

1. Basic hardware multi-channel PWM for **Portenta_H7**.
2. Add Table of Contents

---
---

### Contributions and Thanks

Many thanks for everyone for bug reporting, new feature suggesting, testing and contributing to the development of this library.


---

## Contributing

If you want to contribute to this project:
- Report bugs and errors
- Ask for enhancements
- Create issues and pull requests
- Tell other people about this library

---

### License

- The library is licensed under [MIT](https://github.com/khoih-prog/Portenta_H7_PWM/blob/master/LICENSE)

---

## Copyright

Copyright 2021- Khoi Hoang


