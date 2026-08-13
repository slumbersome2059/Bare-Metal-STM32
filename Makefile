CFLAGS  ?=  -W -Wall -Wextra -Werror -Wundef -Wshadow -Wdouble-promotion \
            -Wformat-truncation -fno-common -Wconversion \
            -g3 -Os -ffunction-sections -fdata-sections -I$(src) \
            -mcpu=cortex-m0plus -mthumb -mfloat-abi=soft $(EXTRA_CFLAGS)
# Some of the CFLAGs are hardware specific(namely -mcpu=... and -mfloat-abi=...) so research the flags required for your processor
LDFLAGS ?= -Ttools/link.ld -nostartfiles -nostdlib --specs nano.specs -lc -lgcc -Wl,--gc-sections -Wl,-Map=build/firmware.elf.map


SOURCES = src/main.c  src/startup.c

build: build/firmware.elf

build/firmware.elf: $(SOURCES) tools/link.ld
	arm-none-eabi-gcc $(SOURCES) $(CFLAGS) $(LDFLAGS) -o $@

# It is enough to make firmware.bin because using Wokwi for simulation
bin/firmware.bin: build/firmware.elf
	arm-none-eabi-objcopy -O binary $< $@

flash: ../bin/firmware.bin
	st-flash --reset write $< 0x8000000

clean:
	rm -rf firmware.* 

# Using stuff like bin/firmware.bin is necessary and VPATH won't
# create output files in required directories and just put them in current directory
