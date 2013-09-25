/**
 * etc/linker.ld
 *
 * Linker script
 * Based on
 * http://wiki.osdev.org/Bare_Bones#Testing_your_operating_system_.28QEMU.29
 *
 * Author: Dennis J. McWherter, Jr.
 * MIT Lincoln Laboratory
 */

ENTRY(boot) /* Kernel entry point */
SECTIONS
{
  . = 0x00100000; /* 1 MB */

  /** 4KB alignment (our small page size) */
  /** BLOCK Advances current loc. ctr to be aligned */
  /** at the start of this section */
  .text ALIGN(0x1000) :
  {
    *(.mbh)
    *(.text)
  }

  .rodata ALIGN(0x1000) :
  {
    *(.rodata)
  }
  
  .data ALIGN(0x1000) :
  {
    *(.data)
  }

  .bss ALIGN(0x1000) :
  {
    *(.stack_init)
    *(.bss)
    *(.COMMON)
  }
}
OUTPUT(bin/kernel.bin)

