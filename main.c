#include <inttypes.h>
const int GPIO_BANK_NUMBER = 5;

//GPIO code

struct GPIO{
    volatile uint32_t MODER, OTYPER, OSPEEDR, PUPDR, IDR, ODR, BSRR, LCKR, AFR[2];
};
typedef struct GPIO GPIO;
typedef enum {
    GPIO_MODE_INPUT,//you will be reading from these registers
    GPIO_MODE_OUTPUT, 
    GPIO_MODE_AF,//you will call some alternate behaviour elsewhere
    GPIO_MODE_ANALOG//you just use the actual analog value of the GPIO pin rather than interpreting it a binary value
    //you could sample the value and take it as input for ADC
} GPIO_mode;

inline GPIO *getGPIO(char bank){
    //GPIO *gpioPins[GPIO_BANK_NUMBER] = {0, 0, 0, 0, 0};
    int base = 0x40020000;
    int offset = 0x400;
    int i = bank - 'A';
    return (GPIO *)(base + i*(offset));
} 
static inline void setModeGPIO(char bank, int pinNum, GPIO_mode gM){
    GPIO *gpioBank = getGPIO(bank);
    //pin numbers start from 0 so you can have A0 and mode for pin number stored as two bits in pinNum*2 and pinNum* + 1
    gpioBank->MODER &= ~(3U << (pinNum*2));//this 0s out what is in bits you will change so that following mask will work and it does not affect other bits
    gpioBank->MODER |= (gM << (pinNum*2));
    //U is added after 3 to make it unsigned to avoid any strange errors that may happen
}

//Startup Code

/*
This code will take stuff in .data and .bss in flash memory and load it into RAM.
This is stuff that needs to be stored in non-volatile memory so that its durable so it will be in flash
but to be used by MCU when chip is booted it needs to be in RAM.
*/

extern long _startDataRAM, _endDataRAM, _startDataFLASH, _startBSS, _endBSS;

__attribute__((naked)) void _reset(void){
    /*
    naked means no function epilogue(restores stack for parent) and no function prologue(this is where stack is set up eg required number of bytes for variables allocated)    
    you use this when you want to write function in assembly code(you can't use any local variables or if statements inside function because of naked)
    you would typically write the prologue and epilogue in assembly
    no_return
    this code will never go back to caller(except when there are exceptions in code)
    difference to void is that function declared with void can go back to caller
    */
    long* src = &(_startDataFLASH);
    for(long* dst = ((&_startDataRAM) + 1); dst < &_endDataRAM; dst++){//addresses of _startDatRAM and endDataRAM are not what you want to copy into they are like goalposts
        *dst = *src;
        src += 1;
    }
    for(long* dst = ((&_startBSS) + 1); dst < &_endBSS; dst++){//addresses of _startDatRAM and endDataRAM are not what you want to copy into they are like goalposts
        *dst = 0;
    }
    //for(;;){0;}//infinite loop

}

extern void _estack(void);

__attribute__((section(".vectors"))) void (* const tab[16+31])(void) = {_estack, _reset};//only first two handlers defined rest are zeroed
//Every handler is a handler for a hardware interrupt, apart from the first two -> second one is the code which starts bootup and first is the initial stack pointer
//So this table is also read at the start for bootup
//There are 16 ARM handlers and 31 peripheral handlers specific to stm32c0-series boards
//The section part means vector table is put in .vectors rather than .text after compilation(we can make linker put .vectors code at start of firmware and so start of flash where it needs to be)
//What is represented by a GPIO_PIN(the 2 bits in all the registers corresponding to one number)?

int main(void){
    _reset();
    return 0;
}