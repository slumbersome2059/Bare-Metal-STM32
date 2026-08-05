In file included from /STMicroelectronics/hardware/stm32/2.8.1/system/Drivers/CMSIS/Device/ST/STM32C0xx/Include/stm32c0xx.h:99,
                 from /STMicroelectronics/hardware/stm32/2.8.1/cores/arduino/../../libraries/SrcWrapper/inc/stm32_def.h:24,
                 from /STMicroelectronics/hardware/stm32/2.8.1/cores/arduino/../../libraries/SrcWrapper/inc/clock.h:19,
                 from /STMicroelectronics/hardware/stm32/2.8.1/cores/arduino/wiring_time.h:23,
                 from /STMicroelectronics/hardware/stm32/2.8.1/cores/arduino/wiring.h:38,
                 from /STMicroelectronics/hardware/stm32/2.8.1/cores/arduino/Arduino.h:36,
                 from sketch.ino.cpp:1:
/STMicroelectronics/hardware/stm32/2.8.1/system/Drivers/CMSIS/Device/ST/STM32C0xx/Include/stm32c031xx.h:500:32: error: expected unqualified-id before numeric constant
  500 | #define PERIPH_BASE           (0x40000000UL)  /*!< Peripheral base address */
      |                                ^~~~~~~~~~~~
/STMicroelectronics/hardware/stm32/2.8.1/system/Drivers/CMSIS/Device/ST/STM32C0xx/Include/stm32c031xx.h:506:32: note: in expansion of macro 'PERIPH_BASE'
  506 | #define AHBPERIPH_BASE        (PERIPH_BASE + 0x00020000UL)
      |                                ^~~~~~~~~~~
/STMicroelectronics/hardware/stm32/2.8.1/system/Drivers/CMSIS/Device/ST/STM32C0xx/Include/stm32c031xx.h:533:32: note: in expansion of macro 'AHBPERIPH_BASE'
  533 | #define RCC_BASE              (AHBPERIPH_BASE + 0x00001000UL)
      |                                ^~~~~~~~~~~~~~
sketch.ino:11:11: note: in expansion of macro 'RCC_BASE'
   11 | const int RCC_BASE = 0x40021000;
      |           ^~~~~~~~
/STMicroelectronics/hardware/stm32/2.8.1/system/Drivers/CMSIS/Device/ST/STM32C0xx/Include/stm32c031xx.h:500:32: error: expected ')' before numeric constant
  500 | #define PERIPH_BASE           (0x40000000UL)  /*!< Peripheral base address */
      |                               ~^~~~~~~~~~~~
/STMicroelectronics/hardware/stm32/2.8.1/system/Drivers/CMSIS/Device/ST/STM32C0xx/Include/stm32c031xx.h:506:32: note: in expansion of macro 'PERIPH_BASE'
  506 | #define AHBPERIPH_BASE        (PERIPH_BASE + 0x00020000UL)
      |                                ^~~~~~~~~~~
/STMicroelectronics/hardware/stm32/2.8.1/system/Drivers/CMSIS/Device/ST/STM32C0xx/Include/stm32c031xx.h:533:32: note: in expansion of macro 'AHBPERIPH_BASE'
  533 | #define RCC_BASE              (AHBPERIPH_BASE + 0x00001000UL)
      |                                ^~~~~~~~~~~~~~
sketch.ino:11:11: note: in expansion of macro 'RCC_BASE'
   11 | const int RCC_BASE = 0x40021000;
      |           ^~~~~~~~
/STMicroelectronics/hardware/stm32/2.8.1/system/Drivers/CMSIS/Device/ST/STM32C0xx/Include/stm32c031xx.h:500:32: error: expected ')' before numeric constant
  500 | #define PERIPH_BASE           (0x40000000UL)  /*!< Peripheral base address */
      |                                ^~~~~~~~~~~~
/STMicroelectronics/hardware/stm32/2.8.1/system/Drivers/CMSIS/Device/ST/STM32C0xx/Include/stm32c031xx.h:506:32: note: in expansion of macro 'PERIPH_BASE'
  506 | #define AHBPERIPH_BASE        (PERIPH_BASE + 0x00020000UL)
      |                                ^~~~~~~~~~~
/STMicroelectronics/hardware/stm32/2.8.1/system/Drivers/CMSIS/Device/ST/STM32C0xx/Include/stm32c031xx.h:533:32: note: in expansion of macro 'AHBPERIPH_BASE'
  533 | #define RCC_BASE              (AHBPERIPH_BASE + 0x00001000UL)
      |                                ^~~~~~~~~~~~~~
sketch.ino:11:11: note: in expansion of macro 'RCC_BASE'
   11 | const int RCC_BASE = 0x40021000;
      |           ^~~~~~~~
/STMicroelectronics/hardware/stm32/2.8.1/system/Drivers/CMSIS/Device/ST/STM32C0xx/Include/stm32c031xx.h:506:31: note: to match this '('
  506 | #define AHBPERIPH_BASE        (PERIPH_BASE + 0x00020000UL)
      |                               ^
/STMicroelectronics/hardware/stm32/2.8.1/system/Drivers/CMSIS/Device/ST/STM32C0xx/Include/stm32c031xx.h:533:32: note: in expansion of macro 'AHBPERIPH_BASE'
  533 | #define RCC_BASE              (AHBPERIPH_BASE + 0x00001000UL)
      |                                ^~~~~~~~~~~~~~
sketch.ino:11:11: note: in expansion of macro 'RCC_BASE'
   11 | const int RCC_BASE = 0x40021000;
      |           ^~~~~~~~
/STMicroelectronics/hardware/stm32/2.8.1/system/Drivers/CMSIS/Device/ST/STM32C0xx/Include/stm32c031xx.h:500:32: error: expected ')' before numeric constant
  500 | #define PERIPH_BASE           (0x40000000UL)  /*!< Peripheral base address */
      |                                ^~~~~~~~~~~~
/STMicroelectronics/hardware/stm32/2.8.1/system/Drivers/CMSIS/Device/ST/STM32C0xx/Include/stm32c031xx.h:506:32: note: in expansion of macro 'PERIPH_BASE'
  506 | #define AHBPERIPH_BASE        (PERIPH_BASE + 0x00020000UL)
      |                                ^~~~~~~~~~~
/STMicroelectronics/hardware/stm32/2.8.1/system/Drivers/CMSIS/Device/ST/STM32C0xx/Include/stm32c031xx.h:533:32: note: in expansion of macro 'AHBPERIPH_BASE'
  533 | #define RCC_BASE              (AHBPERIPH_BASE + 0x00001000UL)
      |                                ^~~~~~~~~~~~~~
sketch.ino:11:11: note: in expansion of macro 'RCC_BASE'
   11 | const int RCC_BASE = 0x40021000;
      |           ^~~~~~~~
/STMicroelectronics/hardware/stm32/2.8.1/system/Drivers/CMSIS/Device/ST/STM32C0xx/Include/stm32c031xx.h:533:31: note: to match this '('
  533 | #define RCC_BASE              (AHBPERIPH_BASE + 0x00001000UL)
      |                               ^
sketch.ino:11:11: note: in expansion of macro 'RCC_BASE'
   11 | const int RCC_BASE = 0x40021000;
      |           ^~~~~~~~
In file included from /STMicroelectronics/hardware/stm32/2.8.1/system/STM32C0xx/stm32c0xx_hal_conf_default.h:211,
                 from /STMicroelectronics/hardware/stm32/2.8.1/system/STM32C0xx/stm32c0xx_hal_conf.h:13,
                 from /STMicroelectronics/hardware/stm32/2.8.1/system/Drivers/STM32C0xx_HAL_Driver/Inc/stm32c0xx_hal.h:30,
                 from /STMicroelectronics/hardware/stm32/2.8.1/system/Drivers/CMSIS/Device/ST/STM32C0xx/Include/stm32c0xx.h:197:
/STMicroelectronics/hardware/stm32/2.8.1/system/Drivers/STM32C0xx_HAL_Driver/Inc/stm32c0xx_hal_gpio.h:118:49: error: expected identifier before '(' token
  118 | #define  GPIO_MODE_INPUT                        (0x00000000U)
      |                                                 ^
sketch.ino:15:5: note: in expansion of macro 'GPIO_MODE_INPUT'
   15 |     GPIO_MODE_INPUT,//you will be reading from these registers
      |     ^~~~~~~~~~~~~~~
/STMicroelectronics/hardware/stm32/2.8.1/system/Drivers/STM32C0xx_HAL_Driver/Inc/stm32c0xx_hal_gpio.h:118:49: error: expected '}' before '(' token
  118 | #define  GPIO_MODE_INPUT                        (0x00000000U)
      |                                                 ^
sketch.ino:15:5: note: in expansion of macro 'GPIO_MODE_INPUT'
   15 |     GPIO_MODE_INPUT,//you will be reading from these registers
      |     ^~~~~~~~~~~~~~~
sketch.ino:14:15: note: to match this '{'
   14 | enum GPIO_mode{
      |               ^
/STMicroelectronics/hardware/stm32/2.8.1/system/Drivers/STM32C0xx_HAL_Driver/Inc/stm32c0xx_hal_gpio.h:118:50: error: expected unqualified-id before numeric constant
  118 | #define  GPIO_MODE_INPUT                        (0x00000000U)
      |                                                  ^~~~~~~~~~~
sketch.ino:15:5: note: in expansion of macro 'GPIO_MODE_INPUT'
   15 |     GPIO_MODE_INPUT,//you will be reading from these registers
      |     ^~~~~~~~~~~~~~~
/STMicroelectronics/hardware/stm32/2.8.1/system/Drivers/STM32C0xx_HAL_Driver/Inc/stm32c0xx_hal_gpio.h:118:50: error: expected ')' before numeric constant
  118 | #define  GPIO_MODE_INPUT                        (0x00000000U)
      |                                                 ~^~~~~~~~~~~
sketch.ino:15:5: note: in expansion of macro 'GPIO_MODE_INPUT'
   15 |     GPIO_MODE_INPUT,//you will be reading from these registers
      |     ^~~~~~~~~~~~~~~
sketch.ino:20:1: error: expected declaration before '}' token
   20 | };
      | ^
sketch.ino: In function 'int main()':
sketch.ino:72:25: error: 'GPIO_MODE_OUTPUT' was not declared in this scope; did you mean 'GPIO_MODE_OUTPUT_OD'?
   72 |     setModeGPIO('A', 2, GPIO_MODE_OUTPUT);
      |                         ^~~~~~~~~~~~~~~~
      |                         GPIO_MODE_OUTPUT_OD

Error during build: exit status 1

