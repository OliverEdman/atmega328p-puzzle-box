![Status](https://img.shields.io/badge/status-work--in--progress-orange)

# Embedded Puzzle Box - Bare metal project


## 🎯 Project Purpose
This project was developed as a core part of my Embedded Systems education. The primary goal was to move beyond high level libraries
and master C programming and Bare Metal development on the AVR architecture.

## 🚀 Overview
* **Memory Game (Simon Says):** Logic-based pattern recognition using tactile buttons and LEDs.
* **Analog Challenge:** High-precision input handling using 4x Potentiometers (ADC).
* **Proximity Task:** Distance measurement using Ultrasonic sensor (Input Capture/Interrupts).
* **User Interface:** Real-time feedback via a 4-digit 7-segment display driven by a 74HC595 shift register (SPI logic).
* **Actuator:** PWM-controlled servo motor for the locking bolt.

## 🛠️ Tech Stack & Hardware
* **Microcontroller:** ATmega328P (8-bit AVR)
* **Language:** Bare Metal C
* **Communication & Logic:** SPI (Shift Register), PWM (Servo), ADC (Potentiometers).
* **Key Components:** 74HC595, SG90 Servo, HC-SR04, 3461AS-1 Display.
* **External Interrupts**

## 📁 Project Structure


```bash
.
├── 📂 app            # Main logic & State Machine
│   └── main.c        # Entry point
├── 📂 drivers        # Hardware Abstraction Layer (HAL)
│   ├── display.c     # 7-segment & 74HC595 driver
│   └── servo.c       # PWM & Timer driver
|
├── 📂 src            # Implementation of puzzles
│   └── functions.c   # Core algorithms & logic
├── 📂 include        # Header files (.h)
│   ├── common.h       <-- Globala defines
│   ├── timer.h        <-- För din klocka
│   ├── adc.h          <-- För potentiometrarna
│   ├── display.h
│   ├── servo.h        <-- Saknades i din lista
│   └── functions.h    <-- Pussel-logikens prototyper
├── 📄 .gitignore     # Files to ignore (hex, obj)
├── 📄 LICENSE        # MIT License
├── 📝 NOTES.md       # Pinout & notes
└── ⚙️ Makefile      # Build configuration
