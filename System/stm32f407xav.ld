/*============= this is a link file for : ===========================
	STM32F407VG
	GNU ARM GCC with Coocox CoIDE
====================================================================*/

OUTPUT_FORMAT ("elf32-littlearm", "elf32-bigarm", "elf32-littlearm")
/* Internal Memory Map*/
MEMORY
{
    rom (rx)  : ORIGIN = 0x08000000, LENGTH = 0x00100000
    ram (rwx) : ORIGIN = 0x20000000, LENGTH = 0x00020000
    ccm (rwx) : ORIGIN = 0x10000000, LENGTH = 0x00010000
}
 
_eram = 0x20000000 + 0x00020000;

/* Section Definitions */
SECTIONS
{
    /* collect all uninitialized .ccm sections */
	/* A directed static allocation would look like this :
		volatile uint8_t buff0[16384] __attribute__((section(".ccm")));   */
	.ccm (NOLOAD) :
    {
        . = ALIGN(4);
        *(.ccm)
        *(.ccm.*)
        . = ALIGN(4);
    } > ccm
 
    .text :
    {
        KEEP(*(.isr_vector .isr_vector.*))
        *(.text .text.* .gnu.linkonce.t.*)
        *(.glue_7t) *(.glue_7)
        *(.rodata .rodata* .gnu.linkonce.r.*)
    } > rom
 
    .ARM.extab :
    {
        *(.ARM.extab* .gnu.linkonce.armextab.*)
    } > rom
 
    __exidx_start = .;
    .ARM.exidx :
    {
        *(.ARM.exidx* .gnu.linkonce.armexidx.*)
    } > rom
    __exidx_end = .;
 
    . = ALIGN(4);
    _etext = .;
    _sidata = .;
 
    .data : AT (_etext)
    {
        _sdata = .;
        *(.data .data.*)
        . = ALIGN(4);
        _edata = . ;
    } > ram
 
    /* .bss section which is used for uninitialized data */
    .bss (NOLOAD) :
    {
        _sbss = . ;
        *(.bss .bss.*)
        *(COMMON)
        . = ALIGN(4);
        _ebss = . ;
    } > ram
 
    /* stack section */
    .co_stack (NOLOAD):
    {
        . = ALIGN(8);
        *(.co_stack .co_stack.*)
    } > ram
 
    . = ALIGN(4);
    _end = . ;
}
