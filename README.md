![Status](https://img.shields.io/badge/status-work--in--progress-orange)

### Embedded Puzzle Box - Bare metal project


### 🎯 Project Purpose
This project was developed as a core part of my Embedded Systems education. The primary goal was to move beyond high level libraries
and master C programming and Bare Metal development on the AVR architecture.

### 🚀 Overview
* **Memory Game (Simon Says):** Logic based pattern recognition using tactile buttons and LEDs.
* **Dual Potentiometer Match:** The player interacts with two potentiometers to control the digits on the 7-segment display (ADC)
* **Distance Calibration:** Distance measurement using Ultrasonic sensor (Input Capture/Interrupts).
* **User Interface:** Real time feedback via a 4 digit 7-segment display driven by a 74HC595 shift register (SPI logic).
* **Password Challange** Password entry system combining a 4x4 Matrix Keypad and an OLED Display via I2C.
* **Servo Lock Mechanism:** The ultimate reward circuit. Upon successful password validation, a dedicated hardware PWM signal a servo motor to physically unlock the box.


### 📁 Project Structure

```bash

.
├── 📂 app
│   └── main.c
├── 📂 drivers               # Hardware Abstraction Layer (HAL)
│   ├── 📂 include           # Header files (Interface) for hardware drivers
│   │   ├── adc.h            
│   │   ├── display_7seg.h   
│   │   ├── display_oled.h   
│   │   ├── distance.h       
│   │   ├── gpio.h           
│   │   ├── servo.h         
│   │   ├── spi.h            
│   │   └── timer.h          
│   └── 📂 source            # Drivers logic implmentation from my Interface 
│       ├── adc.c            
│       ├── display_7seg.c
│       ├── display_oled.c
│       ├── distance.c
│       ├── gpio.c
│       ├── servo.c
│       ├── spi.c
│       └── timer.c
├── 📂 puzzles             
│   ├── 📂 include           # Header files (Interfaces) for all puzzles
│   │   ├── morse.h
│   │   ├── password.h
│   │   ├── pot_to_digits.h
│   │   └── simon_says.h
│   └── 📂 source            # Implementation of puzzle logic
│       ├── morse.c
│       ├── password.c
│       ├── pot_to_digits.c
│       └── simon_says.c
├── 📂 images
│   └── Puzzle_box pins.JPEG
├── 📂 lib              
│   └── utils.h
│
├── 📄 README.md      
├── 📄 .gitignore     
├── 📄 LICENSE        
├── 📝 NOTES.md
└── ⚙️ Makefile 
```


