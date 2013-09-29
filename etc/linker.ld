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

OUTPUT_FORMAT(elf32-i386)
ENTRY(boot) /* Kernel entry point */
SECTIONS
{
  . = 0xc010000d; /* 3GB + 1 MB -- higher-half skipping mbh */

  /** 4KB alignment (our page size) */
  /** BLOCK Advances current loc. ctr to be aligned */
  /** at the start of this section */
  .text : AT(ADDR(.text) - 0xC0000000)
  {
    *(.text)
  }

  .rodata ALIGN(0x1000) : AT(ADDR(.rodata) - 0xC0000000)
  {
    *(.rodata)
  }
  
  .data ALIGN(0x1000) : AT(ADDR(.data) - 0xC0000000)
  {
    *(.data)
  }

  .bss : AT(ADDR(.bss) - 0xC0000000)
  {
    *(.bss)
    *(.COMMON)
  }

  /**
   * These symbols tell use exactly where the end of our
   * memory is. This is good for virtual memory, etc.
   */
  mem_end_virt = .;
  mem_end_phys = . - 0xC0000000;
}
OUTPUT(bin/kernel.bin)

