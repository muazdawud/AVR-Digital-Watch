# Changelog

All notable changes to this project will be documented in this file.

The format of this changelog is inspired by [Keep a Changelog](https://keepachangelog.com/en/1.1.0/), and this project follows a custom development-oriented versioning structure.

---
 
# [v1.0.0] - 2026-05-11

## Initial Release

First complete release of the AVR Digital Watch project featuring a fully interrupt-driven architecture, custom display multiplexing library, and custom DHT11 communication stack.

---

## Added

### Core Watch System
- Implemented a complete digital watch firmware for AVR microcontrollers.
- Added support for:
  - Time display
  - Date display
  - Temperature display
  - Humidity display
- Added automatic time progression using TIMER0 compare-match interrupts.
- Added AM/PM indicator handling.
- Added automatic minute and hour rollover logic.
- Added month/day/year management logic.

---

### Custom 4D_7S Library
- Developed a fully custom 4-digit 7-segment multiplexing library.
- Added TIMER2 interrupt-driven multiplexing.
- Added support for:
  - Decimal point masking
  - PM indicator display
  - Character rendering
  - Flickering digit animation
  - Dynamic display updates
- Added display reset and state recovery logic.
- Added configurable digit-ground mapping using `setPin()`.
- Added optimized multiplexing timing for reduced ISR latency.

---

### Custom DHT_11 Library
- Developed a fully custom DHT11 communication interface.
- Added:
  - Pin Change Interrupt (PCINT) signal decoding
  - Timer-based pulse width analysis
  - Checksum verification
  - Overflow protection
  - Timing guards against rapid polling
- Added support for:
  - Temperature reading
  - Humidity reading
- Added restore-point fallback values on checksum failure.
- Implemented low-level timing analysis using TIMER1.

---

### Interrupt-Driven Architecture
- Implemented ISR-based scheduling for:
  - Time updates
  - Display multiplexing
  - Button press timing
  - DHT11 signal handling
  - Buzzer timing
- Reduced blocking operations throughout the firmware.
- Added atomic operations for shared ISR variables using:
  - `ATOMIC_BLOCK(ATOMIC_FORCEON)`

---

### Setup Interface
- Added setup/configuration mode for:
  - Hour
  - Minute
  - Day
  - Month
  - Year
- Added:
  - Long-click detection
  - Short-click detection
  - Increment handling
  - Flickering edit indicators
- Added setup state machine logic.

---

### Power Management
- Added automatic display shutdown after inactivity.
- Added runtime tracking using overflow counters.
- Added dynamic enabling/disabling of:
  - Display drivers
  - Display grounds
- Added startup/shutdown buzzer notification.

---

### Buzzer System
- Added buzzer activation on:
  - Display startup
  - Power-down events
- Added timer-controlled buzzer timeout logic.

---

### Hardware Support
- Added support for:
  - ATmega328P
  - Common-anode 4-digit 7-segment displays
  - DHT11 sensor
  - Passive buzzer
  - Push-button control interface

---

### Documentation
- Added comprehensive project README.
- Added:
  - Architecture explanation
  - ISR workflow documentation
  - Hardware overview
  - Firmware design notes

---

## Optimized

### Performance
- Reduced display ISR timing overhead.
- Optimized multiplex refresh intervals.
- Reduced unnecessary display refresh operations.
- Minimized runtime polling operations.

---

## Removed

### Cancelled Features
The following planned hardware features were intentionally removed from the final implementation:

- Potentiometer-based brightness adjustment
- LED array subsystem
- Ambient-light adaptive control system

---

### [Unreleased]

---

## Notes

This project was designed as:
- A low-level embedded systems exercise.
- A reusable AVR firmware architecture reference.
- A demonstration of interrupt-driven embedded programming without dependency on external frameworks.

All major libraries included in the project are fully custom/self-developed and designed to operate safely within interrupt-driven environments.

---