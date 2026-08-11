#include <inttypes.h>
#include <stdbool.h>
const int GPIO_BANK_NUMBER = 5;


//GPIO code

struct GPIO{
    volatile uint32_t MODER, OTYPER, OSPEEDR, PUPDR, IDR, ODR, BSRR, LCKR, AFR[2];
};

struct SYS_TICK{
    volatile uint32_t SYST_CSR, SYST_RVR, SYST_CVR, SYST_CALIB;
    
};


typedef struct GPIO GPIO;
typedef struct SYS_TICK SYS_TICK;
typedef enum {
    GPIO_MODE_INPUT,//you will be reading from these registers
    GPIO_MODE_OUTPUT, 
    GPIO_MODE_AF,//you will call some alternate behaviour elsewhere
    GPIO_MODE_ANALOG//you just use the actual analog value of the GPIO pin rather than interpreting it a binary value
    //you could sample the value and take it as input for ADC
} GPIO_mode;

//These describe different registers in the MCU which allow you to enable different peripherals
//They allow clock management and allow you to reset parts of the circuit
const int RCC_BASE = 0x40021000;


//They allow clock management and allow you to reset parts of the circuit
//By configuring registers in the MCU you can enable GPIO banks
//To save power in STM32 all peripherals are turned off but not the case in most other MCUs
//This gives a clock source to components    
uint32_t* RCC_IOPENR = (uint32_t*)(RCC_BASE + 0x34) ;
SYS_TICK* const SYS_TICKp = (SYS_TICK*)0xE000E010;



inline GPIO *getGPIO(char bank){
    //GPIO *gpioPins[GPIO_BANK_NUMBER] = {0, 0, 0, 0, 0};
    int base = 0x50000000;
    int offset = 0x400;
    int i = bank - 'A';
    return (GPIO *)(base + i*(offset));
} 
static inline void setModeGPIO(char bank, int pinNum, GPIO_mode gM){
    GPIO *gpioBank = getGPIO(bank);
    //pin numbers start from 0 so you can have A0 and mode for pin number stored as two bits in pinNum*2 and pinNum* + 1
    gpioBank->MODER &= ~(3U << (pinNum*2));//this 0s out what is in bits you will change so that following mask will work and it does not affect other bits
    gpioBank->MODER |= ((gM & 3U) << (pinNum*2));
    //U is added after 3 to make it unsigned to avoid any strange errors that may happen
}

static inline void writeGPIO(char bank, int pinNum, int val){
    GPIO *gpioBank = getGPIO(bank);
    gpioBank->BSRR = (1U << (pinNum+(val ? 0: 16)));
}
    /*

    - Making a BSRR pin 1 will mean a write or reset happens to the pin number
    on ODR which stores the output and a 0 means ODR will not be assigned
    - if you pass in 0, to write 0 you need to reset so you need to make 1 the bits of 16 to 31
    on BSRR which will reset the bit on ODR
    - There are only 0 to 15 bits on ODR
    - You can modify the ODR directly but writing one bit require a read(taking ODR value 
    into register), modify(bitwise OR), write(back to ODR) series of steps
    - This violates atomicity so after you have read if there is an interrupt and 
    it modifies some other bit of same ODR and then your modify and write happens
    , this would mean that the modification made by interrupt would be lost as you
    modify old read and then write that back
    - With BSRR you directly write(look at the '=') to the register and there is 
    no read or modify loop when you change data of BSRR in the software and in same 
    cycle of writing to BSRR the ODR value is updated as well
    - The whole bit (re)/setting of ODR is done in one cycle so atomic
    */

//Initialises the counting process and returns a success value 
//on whether it worked or not (-1 for failure and 0 for success)
int systickInit(int ticks){
    /*
    - Set the reload value in SYST_RVR
    - Clear SYST_CVR(this means write it to 0 -> writitng ANY VALUE to SYST_CVR will
     clear it to 0)
    - Enable the counting process through SYST_CSR
    - For every cycle, it will (detect if CVR is 0 and so reset to value in SYS_RVR 
    then do following) decrement the value in SYST_CVR and there is a trigger that activates 
    when counter decrements from 1 to 0 which sets the SYSTICK exception pending status to true
     which should run the exception handler when possible
    */
    if(ticks - 1 > 0xffffff || ticks <= 1){
        return -1;//the systick register for the current value has 32 bits but only 24 of them are used for storage
    }
    SYS_TICKp->SYST_RVR = (uint32_t)(ticks-1);//One cycle is used to run the interrupt handler so you subtract off one for the cycles that main runs
    SYS_TICKp->SYST_CVR = 0;//clrea
    SYS_TICKp->SYST_CSR = 0x7;
    /*
    - this is 111 becuase when 0 bit is 1 you enable counting, 
    - 1st bit is 1 means interrupt generation and you call handler 
    - 2nd bit is 1 means you use processor clock
    */
   return 0;
    
}

static volatile uint32_t s_ticks = 0; 
void systickHandler(){
    s_ticks++;
}

/*
- Don't create an expire function like this because it does not work with overflows
- So for example when expirationTime is very high and timeNow reaches it 
,expirationTime goes very low and we keep returning true because second
 if check won't succeed
- There's also the problem that when timeNow overlaps so much that we just keep returning false
- The way to deal with is to avoid the comparison between timeNow and expirationTime 
bool timerExpire(uint32_t* expirationTime, uint32_t prd, uint32_t timeNow){
    if(*expirationTime == 0){//for first time that timerExpire called
        *expirationTime = timeNow + prd;
    }
    if(timeNow < *expirationTime){
        return false;
    }
    *expirationTime = prd - (timeNow - *expirationTime)%(prd) +timeNow; 
    return true;
}
*/

bool timerExpire(uint32_t* lastTick, uint32_t prd, uint32_t timeNow){
    bool entered = false;
    while(timeNow - *lastTick >= prd){
        entered = true;
        *lastTick += prd;
    }
    return entered;
}



int main(void){
    systickInit(48000);
    *RCC_IOPENR |= 1;
    setModeGPIO('A', 10, GPIO_MODE_OUTPUT);
    
    static bool on = true;
    uint32_t prd = 2000;
    uint32_t lastTick = s_ticks;
    while(1){
        if(timerExpire(&lastTick, prd, s_ticks)){
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

//Startup Code
extern long _startDataRAM, _endDataRAM, _startDataFLASH, _startBSS, _endBSS;

/*
This code will take stuff in .data and .bss in flash memory and load it into RAM.
This is stuff that needs to be stored in non-volatile memory so that its durable so it will be in flash
but to be used by MCU when chip is booted it needs to be in RAM.
*/

__attribute__((naked, noreturn)) void _reset(void){
    /*
    naked 
    - means no function epilogue(restores stack for parent) and no function prologue(this is where stack is set up eg required number of bytes for variables allocated)    
    - you use this when you want to write function in assembly code(you can't use any local variables or if statements inside function because of naked)
    - you would typically write the prologue and epilogue in assembly
    - The code in this function should be in assembly in a __asm__{code} statement
    - Sometimes C code will work but its unreliable and should never be trusted(we get lucky here)
    - Prologue also saves the current context like registers but as this function is 
    run at the start the registers will have dummy data so storing them in RAM is a waste of time
    - Epilogue also not necessary(an epilogue is generated even though no return) so naked
    can save space
    
    
    no_return
    - this code will never go back to caller(except when there are exceptions in code)
    difference to void is that function declared with void can go back to caller
    - mainly used for optimisations like cutting out code that is written after the function call(that code will never execute)
    */
    long* src = &(_startDataFLASH);
    /*
    - Linker script variables add the variableName to the symbol table and the
    value that the variable is assigned to, is stored in an entry with the variableName 
    in the symbol table(so the value that the variableName is assigned to is its address not value), 
    typically for declarations in C in symbol table you would store varName = memory address of value stored
    but with a linker script variable there is no memory assigned to the variable
    - This is why in source code you access &_startDataRAM and same for other linker 
    script variables because all you want is the address of the variable because the value of 
    linker script variable is stored where you would typically store an address of a typical variable
    - using the value of a variable in source code(so if you have
    int a = 5; int b = a + 2) will get address of a from symbol table and use the 
    address to get the value of a 
    - So plainly using _startDataRam will cause problems because there is no memory
    assigned to _startDataRam so you can't access a value like you typically would for 
    high level variables
    */
    for(long* dst = ((&_startDataRAM)); dst < &_endDataRAM; dst++){//addresses of _startDatRAM and endDataRAM are not what you want to copy into they are like goalposts
        *dst = *src;
        src += 1;
    }
    for(long* dst = ((&_startBSS)); dst < &_endBSS; dst++){//addresses of _startDatRAM and endDataRAM are not what you want to copy into they are like goalposts
        *dst = 0;
    }
    main();
    for(;;);//infinite loop

};

extern void _estack(void);

__attribute__((used, section(".vectors"))) void (* const tab[16+31])(void) = {_estack, _reset, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, systickHandler};
//only first two handlers defined rest are zeroed
//Every handler is a handler for a hardware interrupt, apart from the first two -> second one is the code which starts bootup and first is the initial stack pointer
//So this table is also read at the start for bootup
//There are 16 ARM handlers and 31 peripheral handlers specific to stm32c0-series boards
//The section part means vector table is put in .vectors rather than .text after compilation(we can make linker put .vectors code at start of firmware and so start of flash where it needs to be)
//What is represented by a GPIO_PIN(the 2 bits in all the registers corresponding to one number)?
/*
- Also in the symbol table the function name will be stored 
with the address of the function so you can just pass the function name in and you dont have to do &_estack
- This is the same thing with array names as well so if you had 
arrayName[] and then do arrayName = .; in linker script you would just access 
arrayName for the address
- used attribute is to make the compiler write code for function even if it is 
not referenced(tab is used in bootup and invisible to compiler)
- I haven't seen volatile to defend against what the used attribute does
*/
