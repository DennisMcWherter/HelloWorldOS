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
  . =  0x00100000;
  
  /** 4KB alignment (our page size) */
  /** BLOCK Advances current loc. ctr to be aligned */
  /** at the start of this section */
  .text : 
  {
    *(.text)
  }

  . += 0xc0000000;

  .rodata ALIGN(0x1000) : AT(ADDR(.rodata) - 0xc0000000)
  {
    *(.rodata)
  }
  
  .data ALIGN(0x1000) : AT(ADDR(.data) - 0xc0000000)
  {
    *(.data)
  }

  .bss : AT(ADDR(.bss) - 0xc0000000)
  {
    *(.bss)
    *(.COMMON)
  }

  /**
   * These symbols tell use exactly where the end of our
   * memory is. This is good for virtual memory, etc.
   */
  _mem_end_virt = .;
  _mem_end_phys = . - 0xc0000000;
}
OUTPUT(bin/kernel.bin)

