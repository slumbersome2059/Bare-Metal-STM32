# README
## Summary of Project
This repository is for a project that supports a blinking LED that blinks for reliable periods and it supports UART output on the serial monitor. To ensure blinking for a reliable period I learnt to use the SYSTICK peripheral. I did not use any normal libraries and wrote all the code for the startup and hardware abstraction layer. I also learnt how to use linker script to load the firmware into memory. Furthermore, I enhanced my knowledge on Makefile fundamentals in this project. The tutorial https://github.com/cpq/bare-metal-programming-guide/tree/main was very helpful for this project. My board was different to the one on the guide so I had to use the datasheet to figure out how to use the new registers on my board.
## Skills Learnt
- Makefile
- Linker Script
- UART
- SYSTICK
- GPIO
## Running the project
Load the binary file into the Wokwi simulator given here: https://wokwi.com/projects/new/st-nucleo-c031c6
