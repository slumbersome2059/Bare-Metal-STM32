/*this linker script maps the compiled firmware code to flash memory*/

/*
This is for debugger to know which function runs first 
so that it can set a breakpoint at the beginning of firmware.
*/
ENTRY(_reset); 


/*
Even though on datasheet it says first entry of vector table starts at 0x00000000
and we are mapping it to 0x08000000. The memory from 0x0 is an alias so when
something from that region is required eg 0x0 the hardware forwards request to address 
it is aliasing so in this case 0x08.
What the memory from 0x0 aliases to is determined by mode set on BOOT pins and 
by default it is flash but it can also be system memory or SRAM.
Only the first two interrupt handlers are aliased actually and not all because 
once these are accessed the program code will be read and executed and here 
it will set a VTOR register which will have offset of 0x08000000 and then when 
interrupt vectors are accessed this offset will be added so there is no aliasing for the other interrupts.
*/
MEMORY{
/*
The rx and rwx specify permissions.
*/
    flash(rx): ORIGIN = 0x08000000, LENGTH = 32k
    sram(rwx): ORIGIN = 0x20000000, LENGTH = 12k
}
/*The stack points where sram ends*/
_estack = ORIGIN(sram) + LENGTH(sram);

SECTIONS {
    .vectors : {* (.vectors)} > flash
    .text : {* (.text*)} > flash
    .rodata : {* (.rodata*)} > flash /* rodata is not stored in RAM for space */
    /* 
    Each of the LHS is creating a section in the output file called that secname
    and taking from all the files(what the * does) the code in specified section inside
    to the current section. So it adds all the code in .vectors section in all the files to .vectors of output fie 
    Also the format in section matches how declarations come up
    So .vectors first, then .text, ... in the actual memory
    */
    .data : {
        /* data section holds initialised global variables */
        _startDataRAM = .;/* Semicolon must follow an assignment */
        * (.firstData)
        *(.data SORT(.data.*)) /*
        - Sometimes you can specify to compile code 
        so that each variable is in its own section which makes it easier to remove
        if a variable is unused meaning you can save space 
        - SORT means all these sections 
        get's added in a consistent order no matter which order files processed
        - .data is added first and then these .data.* sections added
        */
        _endDataRAM = .;
    } > sram AT > flash
    _startDataFLASH = LOADADDR(.data);
    /*
    - The . is the output location counter so it stores the thing at the end of
    the data written. The > sram controls what the . is set to so here the . is set
    to the start of sram because of > sram. 
    - The assignment _startDataRAM = ... creates a variable which you can use later
    - The AT > flash tells you where to load the data in, if there is no AT the
    load address = address based from the >(so for the >flash, it records what's 
    added to the flash to generate the virtual address at the end of what's added 
    and then as there is no AT it just assumes that the virtual address is load address)
    - _startDataRAM, ... and the other _ can be accessed as variables -> they will
    be used in the copy code of the flash to the RAM
    - Currently nothing in code uses .firstData as a section so it just gets ignored
    */
    .bss : {/* unitialised global variables will be given 0 by default*/
        _startBSS = .;
        *(.bss SORT(.bss.*)) /* this and next line have same meaning as above with the data*/
        
        _endBSS = .;
    } > sram 
    /*
    - This is never loaded into flash(no AT > flash)! We already know what initial 
    values these variables will take(0) so there is no need to load it into flash.
    - We just do this to figure out where it starts and ends so we can write 0s
    */

}