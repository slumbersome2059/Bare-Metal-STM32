//This is the hardware abstraction layer, it gives structures which can be used to access the memory to which registers are mapped to
//It also usually includes functions to interact with hardware
#include <inttypes.h>
//GPIO code
struct GPIO{
    volatile uint32_t MODER, OTYPER, OSPEEDR, PUPDR, IDR, ODR, BSRR, LCKR, AFRL, AFRH;
};

struct SYS_TICK{
    volatile uint32_t SYST_CSR, SYST_RVR, SYST_CVR, SYST_CALIB;  
};

struct UART{
    volatile uint32_t CR1, CR2, CR3, BRR, GTPR, RTQR, RQR, ISR, ICR, RDR, TDR, PRESC;
    /*
    - BRR -> set the baud rate, for UART and USART this is the amount of data transmitted per second
    - GTPR -> sets the guard time value(transmission complete flag set after this 
    time is drained), and sets prescaler value which divides system clock -> both these values are only
     accessed in certain modes and not in normal mode
    - RTOR -> receiver timeout register, sets a flag after some time 
    where nothing is to be read
    - RQR -> used to make requests like discard data without reading it, or put USART in mute mode
    - ISR -> gives information on the status of the USART like a busy flag if there is comms on the RX line, or RX stack is full
    - ICR -> clears flags of ISR, same idea of BSRR used for ODR
    - RDR -> contains data character received
    - TDR -> contains data character to be transmitted
    - PRESC -> used to divide the input clock by some number 
    */
};

typedef struct GPIO GPIO;
typedef struct SYS_TICK SYS_TICK;
typedef struct UART UART;
typedef enum {
    GPIO_MODE_INPUT,//you will be reading from these registers
    GPIO_MODE_OUTPUT, 
    GPIO_MODE_AF,//this maps the pin as input for or output of(whether I or O depends on AF number and the pin) some other peripheral like USART, SPI, ...
    GPIO_MODE_ANALOG//you just use the actual analog value of the GPIO pin rather than interpreting it a binary value
    //you could sample the value and take it as input for ADC
} GPIO_mode;
SYS_TICK* const SYS_TICKp = (SYS_TICK*)0xE000E010;

static inline int powInt(int base, int exp){
    int ans = 1;
    while(exp){
        ans *= base;
        exp--;
    }
    return ans;
}

/*
I think this should not have been made into a function, code like 5*2 will now have to be executed
and pow will be executed so it would have been more efficient to just call the normal code.
*/
static inline void setReg(volatile uint32_t* reg, int index, int stride, int val){
    int oneMask = (int)((powInt(2, stride)) - 1);
    *reg &= (uint32_t)(~(((oneMask)) << (index*stride)));//this 0s out what is in bits you will change so that following mask will work and it does not affect other bits
    *reg |= (uint32_t)(((val)&(oneMask)) << (index*stride));
}

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
    setReg(&(gpioBank->MODER), pinNum, 2, gM);
    //U is added after 3 to make it unsigned to avoid any strange errors that may happen
}

static inline void setAltFuncGPIO(char bank, int pinNum, int afNum){
    GPIO *gpioBank = getGPIO(bank);
    if(pinNum <= 7){
        setReg(&(gpioBank->AFRL), pinNum, 4, afNum);
    }else if(pinNum >= 8 && pinNum <= 15){
        setReg(&(gpioBank->AFRH), pinNum - 8, 4, afNum);//setReg requires an index so you need to - 8
    }
    
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
    into register as you want to preserve the values in other bits), modify(bitwise 
    OR), write(back to ODR) series of steps
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

bool timerExpire(uint32_t* lastTick, uint32_t prd, uint32_t timeNow){
    bool entered = false;
    while(timeNow - *lastTick >= prd){
        entered = true;
        *lastTick += prd;
    }
    return entered;
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