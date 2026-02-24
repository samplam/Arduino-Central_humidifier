# Arduino-Central_humidifier
Program that was coded to run on an Arduino Nano board. It's used as a "PLC" for a heating element based central air humidifier.

## Overview
This project is an Arduino-based that I made to control the central air humidifier that I made completely from scratch in stainless. The humidifer is composed of a "one wire" temperature sensor, a manual fill button, a manual flush button, a fill valve, a flush valve, an audible alarm, a low level float, a high level float and a heating element. The program also features multiple fault status. It also features an automatic "end of humifying season" mode, an automatic flushing logic after a specified number of fills and a way to know if the air exchanger has stopped by measuring the temperature at the air input of the humidifier (to avoid condensation in the ventilation ducts).

## Features
- Basic automation loops and logic  
- Commented and structured code for readability

## Skills Highlighted
- Programming in C/C++ for microcontrollers
- Basic electronics and automation principles

## Usage
1. Open `Humidifier_v2020-04-17.ino` in Arduino IDE
2. Upload to the Arduino board
3. Connect the input and output as per requirements
4. Test the program

## Notes
- Written in 2020 before my formal IT studies; shows structured code and practical electronics knowledge. Also note that no functions were used and that the core of the code is based around several nested IF structures.
