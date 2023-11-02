/* Script for -z combreloc */
/* 
    Copyright (C) 2014-2020 Free Software Foundation, Inc.
    Copying and distribution of this script, with or without modification,
    are permitted in any medium without royalty provided the copyright
    notice and this notice are preserved.  
*/
OUTPUT_FORMAT("elf32-littleriscv", "elf32-littleriscv",
	      "elf32-littleriscv")
OUTPUT_ARCH(riscv)
MEMORY
{
    /* riscv-console virt machine FLASH */
    FLASH (rx)  : ORIGIN = 0x20000000, LENGTH = 16M
    /* riscv-console virt machine RAM */
    RAM (rwx)   : ORIGIN = 0x70800000, LENGTH = 8M
    
}
ENTRY(_start)
SEARCH_DIR("/opt/riscv32/riscv32-unknown-elf/lib");
SECTIONS
{
    /* 
        Based off of script at the following URLs:
        https://twilco.github.io/riscv-from-scratch/2019/04/27/riscv-from-scratch-2.html
        https://microcontrollerslab.com/bare-metal-embedded-systems-linker-script-file/
    */
    /* 
        initial stack pointer pointing to top of stack,starts from higher addresses
        i.e ORIGIN(RAM) + LENGTH(RAM)-1 until it comes in contact with .bss or .data 
        in which case a buffer overflow occurs
    */
    PROVIDE( __stack_top = ORIGIN(RAM) + LENGTH(RAM));

    /* constants  and other code stored in FLASH */
    .text :
    {
        _text = .;              /* beginning of .text segment,also called code memory */
        /*KEEP(*(.vector_table))  /* vector table defined in startup.c to be included */
        *(.init*)               /* entry point needs to be a base */
        *(.text*)               /* other code */
        _etext = .;             /* end of .text segment */
    } > FLASH

    .rodata :
    {
        _rodata = .;
        *(.rodata*)             /* constants go here */
        *(.srodata*)            /* constants go here */
        . = ALIGN(4);
        _erodata = .;
    } > FLASH

    /* data, initialized variables, to be copied to RAM after reset */
    .data : 
    {
        _data = .;          /* beginning of .data segment */
        *(.data*)           /* data goes here */
        . = ALIGN(4);
        _edata = .;         /* end of .data segment */
    } > RAM AT >FLASH       /* .data segment starts directly after the .text section in FLASH */

    /* sdata, initialized variables, to be copied to RAM after reset */
    .sdata : 
    {
        _sdata = .;         /* beginning of .sdata segment */
        *(.sdata*)          /* data goes here */
        . = ALIGN(4);
        _esdata = .;        /* end of .data segment */
    } > RAM AT >FLASH       /* .sdata segment starts directly after the .text section in FLASH */

    /* uninitialized data which is initialized to 0 upon after reset */
    .bss :
    {
        _bss = .;           /* beginning of .bss segment */
        *(.bss*)            /* .bss content goes here */
        *(.sbss*)           /* .sbss content goes here */
        *(COMMON)       
        . = ALIGN(. != 0 ? 32 / 8 : 1);
        _ebss = .;          /* end of .bss segment */
    } > RAM

    __global_pointer$ = MIN(_sdata + 0x800,
		            MAX(_data + 0x800, _ebss - 0x800));

}
