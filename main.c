#include <inttypes.h>
 //bank is just GPIO A, GPIO B, ... 
//Set mode of GPIO with any from modes(r, w, ...), with offset passed in
//Set mode by passing in bank, pinNum
//Define readable banks, readable modes, gpio
const int GPIO_BANK_NUMBER = 5;
struct GPIO{
    volatile uint32_t MODER, OTYPER, OSPEEDR, PUPDR, IDR, ODR, BSRR, LCKR, AFR[2];
};
typedef struct GPIO GPIO;
enum GPIO_mode {
    GPIO_MODE_INPUT,//you will be reading from these registers
    GPIO_MODE_OUTPUT, 
    GPIO_MODE_AF,//you will call some alternate behaviour elsewhere
    GPIO_MODE_ANALOG//you just use the actual analog value of the GPIO pin rather than interpreting it a binary value
    //you could sample the value and take it as input for ADC
};
typedef GPIO_mode GPIO_mode;
inline GPIO *getGPIO(char bank){
    //GPIO *gpioPins[GPIO_BANK_NUMBER] = {0, 0, 0, 0, 0};
    int base = 0x40020000;
    int offset = 0x400;
    int i = bank - 'A';
    return (GPIO *)(base + i*(offset));
} 
static inline void setModeGPIO(char bank, int pinNum, GPIO_mode GPIO_mode){
    GPIO *gpioBank = getGPIO(bank);
    //pin numbers start from 0 so you can have A0 and mode for pin number stored as two bits in pinNum*2 and pinNum* + 1
    gpioBank->MODER &= ~(3U << (pinNum*2));//this 0s out what is in bits you will change so that following mask will work and it does not affect other bits
    gpioBank->MODER |= (GPIO_mode << (pinNum*2));
    //U is added after 3 to make it unsigned to avoid any strange errors that may happen
}


//What is represented by a GPIO_PIN(the 2 bits in all the registers corresponding to one number)?