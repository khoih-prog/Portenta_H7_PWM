# Portenta_H7_PWM Library

[![arduino-library-badge](https://www.ardu-badge.com/badge/Portenta_H7_PWM.svg?)](https://www.ardu-badge.com/Portenta_H7_PWM)
[![GitHub release](https://img.shields.io/github/release/khoih-prog/Portenta_H7_PWM.svg)](https://github.com/khoih-prog/Portenta_H7_PWM/releases)
[![GitHub](https://img.shields.io/github/license/mashape/apistatus.svg)](https://github.com/khoih-prog/Portenta_H7_PWM/blob/master/LICENSE)
[![contributions welcome](https://img.shields.io/badge/contributions-welcome-brightgreen.svg?style=flat)](#Contributing)
[![GitHub issues](https://img.shields.io/github/issues/khoih-prog/Portenta_H7_PWM.svg)](http://github.com/khoih-prog/Portenta_H7_PWM/issues)

---
---

## Table of Contents

* [Changelog](#changelog)
  * [Releases v2.0.2](#Releases-v202)
  * [Releases v2.0.1](#Releases-v201)
  * [Releases v2.0.0](#Releases-v200)
  * [Initial Releases v1.0.0](#Initial-Releases-v100)

---
---

## Changelog

### Releases v2.0.2

1. Convert to `h-only` code
2. Optimize library code by using `reference-passing` instead of `value-passing`
3. Add example [multiFileProject](examples/multiFileProject) to demo for multiple-file project
4. Update `Packages_Patches` and test with new ArduinoCore-mbed `mbed_portenta core v2.7.2`

### Releases v2.0.1

1. Fix PWM_Multi example by not using HRTIM pinD3 and pinD6 at the same time
2. Temporary fix polarity for HRTIM PWM for core v2.6.1-. Check [Portenta: PWM on HRTIM has inverted corner case values #352](https://github.com/arduino/ArduinoCore-mbed/issues/352). To be modified when the issue is fixed.

### Releases v2.0.0

1. Completely new code to fix no PWM output issue. Check [No PWM-Output signals #1](https://github.com/khoih-prog/Portenta_H7_PWM/issues/1)
2. To permit start, stop, change and restore the settings of any PWM channel on-the-fly

### Initial Releases v1.0.0

1. Initial coding to support **Portenta_H7 boards** such as Portenta_H7 Rev2 ABX00042, etc., using [**ArduinoCore-mbed mbed_portenta** core](https://github.com/arduino/ArduinoCore-mbed)


---
---

## Copyright

Copyright 2021- Khoi Hoang


