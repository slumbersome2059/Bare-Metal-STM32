# The makefile came from https://github.com/cpq/bare-metal-programming-guide/blob/main/steps/step-0-minimal/Makefile
CFLAGS  ?=  -W -Wall -Wextra -Werror -Wundef -Wshadow -Wdouble-promotion \
            -Wformat-truncation -fno-common -Wconversion \
            -g3 -Os -ffunction-sections -fdata-sections -I. \
            -mcpu=cortex-m0plus -mthumb -mfloat-abi=soft $(EXTRA_CFLAGS)
# Some of the CFLAGs are hardware specific(namely -mcpu=... and -mfloat-abi=...) so research the flags required for your processor
LDFLAGS ?= -Tlink.ld -nostartfiles -nostdlib --specs nano.specs -lc -lgcc -Wl,--gc-sections -Wl,-Map=$@.map


SOURCES = main.c 

build: firmware.elf

firmware.elf: $(SOURCES) link.ld
	arm-none-eabi-gcc $(SOURCES) $(CFLAGS) $(LDFLAGS) -o $@

# It is enough to make firmware.bin because using Wokwi for simulation
firmware.bin: firmware.elf
	arm-none-eabi-objcopy -O binary $< $@
	

flash: firmware.bin
	st-flash --reset write $< 0x8000000

clean:
	rm -rf firmware.*