ENTRY(__reset_handler);

FLASH_VADDR = 0x08000000;
FLASH_SIZE  = 0x00010000;
SRAM_VADDR  = 0x20000000;
SRAM_SIZE   = 0x00002000;

SECTIONS
{
    . = 0x00000000;

    .text : AT(ADDR(.text) + FLASH_VADDR)
    {
        KEEP(*(.vector_table));
        *(.text)
        *(.rodata)
    }

    __stack_start = SRAM_VADDR + SRAM_SIZE;

    /DISCARD/ :
    {
        *(.ARM.attributes)
        *(.comment)
        *(.symtab)
    }
}
