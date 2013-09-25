##
# x86/boot.s
#
# This is where the magic happens to boot
# from grub.
#
# Multiboot specification information:
# http://www.gnu.org/software/grub/manual/multiboot/multiboot.html#Machine-state
#
# Author: Dennis J. McWherter, Jr.
# MIT Lincoln Laboratory
##
.intel_syntax noprefix # If prefer AT&T syntax, but I imagine mostly everyone else uses intel.

# External C methods
.extern kernel_init
.extern kernel_main

# Some defines to make things a little easier to read/understand
# Review section 3.1.2 of mb spec
.set MAGICNO,  0x1badb002 # Magic number
.set ALIGN,    0x01 # Align on 4KB
.set MEMDATA,  0x02 # Make mem_* data present
.set FLAGS, ALIGN | MEMDATA # Combined flags to single var for clarity
.set CHECKSUM, -(MAGICNO + FLAGS)

.section .mbh
# Layout for mb header
# Review section 3.1.1 of mb spec
.align 4 # 4-byte alignment
.long MAGICNO
.long FLAGS
.long CHECKSUM

# Initialize a stack
.set STACK_SIZE, 0x4000 # 2^14 = 16KB
stack_init:
.comm stack, STACK_SIZE

.section .text
.globl boot
boot:
  mov esp, (stack + STACK_SIZE)

  cli # Disable interrupts

  # Call our init method
  # int kernel_init(int magic, multiboot_info_t* info)
  push ebx # Multiboot structure
  push eax # Magic number
  call kernel_init
  add esp, 8 # Pop args
  test eax, eax # Make sure init_kernel() returns 0
  jnz end

  sti # Re-enable interrupts

  # void kernel_main()
  call kernel_main # Main function

end:
  cli # Disable interrupts
 
halt:
  hlt
  jmp halt


