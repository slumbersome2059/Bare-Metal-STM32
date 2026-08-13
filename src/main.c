//#define CLOCK_FREQ 12000000UL
#include <inttypes.h>
#include <stdbool.h>
#include "hal.h"

#define USARTDIV 48000000/115200
const int GPIO_BANK_NUMBER = 5;
const int CLOCK_FREQ = 48000000;



//These describe different registers in the MCU which allow you to enable different peripherals
//They allow clock management and allow you to reset parts of the circuit
const int RCC_BASE = 0x40021000;


//They allow clock management and allow you to reset parts of the circuit
//By configuring registers in the MCU you can enable GPIO banks
//To save power in STM32 all peripherals are turned off but not the case in most other MCUs
//This gives a clock source to components    
uint32_t* RCC_IOPENR = (uint32_t*)(RCC_BASE + 0x34) ;
uint32_t* RCC_APBENR1 = (uint32_t*)(RCC_BASE + 0x3C) ;

//Only uart1 and uart2 have RX and TX ports mapped to GPIO pins on stm32c031c6 
UART* const uart1 = (UART*)0x40013800;
UART* const uart2 = (UART*)0x40004400;
UART* const uart3 = (UART*)0x40004800;
UART* const uart4 = (UART*)0x40004C00;


static volatile uint32_t s_ticks = 0; 
void systickHandler(){
    s_ticks++;
}



void delay(int N){
    while(N--){
        asm("nop");
    }
}

void initSerialMonitor(){
    //Setting up GPIO pins
    setModeGPIO('A', 2, GPIO_MODE_AF);
    setModeGPIO('A', 3, GPIO_MODE_AF);
    setAltFuncGPIO('A', 2, 1);
    setAltFuncGPIO('A', 3, 1);
    //Enabling USART peripheral
    *RCC_APBENR1 |= (1 << 17); //you are only changing one bit so no need to zero things out(would be necessary for storing eg 01)
    (void)*RCC_APBENR1; // Dummy read forces CPU to wait for clock stabilization
    uart2->CR1 = 0;//UART(from setting UE bit to 0) needs to be disabled for some bits to be set
    uart2->BRR = (uint32_t)(48000000/115200);
    uart2->CR1 = 13;
}

void writeToSerialMonitor(char* msg){
    while(*msg != 0){
        uart2->TDR = (uint8_t)(*msg);
        msg++;
        while((uart2->ISR & (1 << 7)) == 0){
            /*
            This is TXE bit which is used to show TDR is free and the data in there has 
            been moved to shift register so you can write there
            There is a TC bit which shows the whole transmission is complete so shift register is empty 
            and TX line is IDLE. This is used right at the end so that you don't disable 
            the USART when there is data in shift register for example. But TC does not seem to work
             on STM32 when I use it here. For example, using TC should mean loop runs longer but no 
             change in output while only first letter gets printed in reality.
            */
            delay(1);
        };
    }
    
}

int main(void){
    systickInit(CLOCK_FREQ/1000);
    *RCC_IOPENR |= 1;
    setModeGPIO('A', 10, GPIO_MODE_OUTPUT);
    initSerialMonitor();
    writeToSerialMonitor("HELLO\n");
    
    static bool on = true;
    uint32_t prd = 2000;
    uint32_t lastTick = s_ticks;
    while(1){
        if(timerExpire(&lastTick, prd, s_ticks)){
            writeToSerialMonitor("HELLO\n");
            writeGPIO('A', 10, on);
            on = !on;
        }
        /*
        - you can do other stuff here -> benefit of doing this compared to delay 
        from empty loop
        - but delay loop better for accuracy because if the code below takes a 
        considerable time the next LED blink will have to pay for it
        */
    }
    return 0;
}

