ENTRY(__reset_handler);

FLASH_VADDR = 0x00000000;
FLASH_PADDR = 0x08000000;
FLASH_SIZE  = 0x00010000;
SRAM_VADDR  = 0x20000000;
SRAM_PADDR  = 0x20000000;
SRAM_SIZE   = 0x00002000;

MEMORY
{
    FLASH  (rx)  : ORIGIN = FLASH_VADDR, LENGTH = FLASH_SIZE
    SRAM   (rwx) : ORIGIN =  SRAM_VADDR, LENGTH =  SRAM_SIZE
}

SECTIONS
{
    . = 0x0;

    .text :
    {
        KEEP(*(.vector_table));
        *(.text)
        *(.rodata)
    } > FLASH

    .data :
    {
        . = ALIGN(4);
        __data_start_vma = .;

        *(.data)

        . = ALIGN(4);
        __data_end_vma = .;
    } >SRAM AT >FLASH

    .bss :
    {
        . = ALIGN(4);
        __bss_start_vma = .;

        *(.bss)
        *(.bss*)

        . = ALIGN(4);
        __bss_end_vma = .;
    } > SRAM AT >FLASH

    __data_start_lma = LOADADDR(.data);
    __bss_start_lma  = LOADADDR(.bss);

    __stack_start = SRAM_VADDR + SRAM_SIZE;

    /DISCARD/ :
    {
        *(.ARM.attributes)
        *(.comment)
        *(.symtab)
    }
}
