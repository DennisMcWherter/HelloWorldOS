##
# x86/boot.s
#
# This is where the magic happens to boot
# from grub.
#
# Multiboot specification information:
# http://www.gnu.org/software/grub/manual/multiboot/multiboot.html#Machine-state
#
# We will also enable paging in here to boot in
# the higher-half of memory space
#
# Author: Dennis J. McWherter, Jr.
# MIT Lincoln Laboratory
##
.intel_syntax noprefix # If prefer AT&T syntax, but I imagine mostly everyone else uses intel.

##
# External C methods
##
.extern kernel_init
.extern kernel_main
.extern write
.extern paging_enable

##
# Multiboot defines
##
# Some defines to make things a little easier to read/understand
# Review section 3.1.2 of mb spec
.set MAGICNO,  0x1badb002 # Magic number
.set ALIGN,    0x01 # Align on 4KB
.set MEMDATA,  0x02 # Make mem_* data present
.set FLAGS, ALIGN | MEMDATA # Combined flags to single var for clarity
.set CHECKSUM, -(MAGICNO + FLAGS)

##
# Code section
##
#.section .text
# Layout for mb header
# Review section 3.1.1 of mb spec
#.align 4 
#.long MAGICNO
#.long FLAGS
#.long CHECKSUM
.section .text
# Layout for mb header
# Review section 3.1.1 of mb spec
.align 4 
.long MAGICNO
.long FLAGS
.long CHECKSUM

.globl boot
boot: # Address is virtual, so we call "boot" instead since we need to enable paging
  # Setup our stack with its physical address first
  mov esp, (stack + STACK_SIZE)
  mov ebp, esp

  # Our addresses are all virtual which is not good
  # for us now since paging isn't enabled yet.
  # Subtract virtual offset and access directly!
  push eax
  call paging_enable
  pop eax

  # Now do a long jump into the higher half
  jmp (boot_higherhalf+0xc0000000)

boot_higherhalf:
  mov esp, (stack + STACK_SIZE + 0xc0000000)
  mov ebp, esp

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
  push exit_str
  call write
  add esp, 4
 
halt:
  hlt
  jmp halt

##
# Read-only data
##
.section .rodata
exit_str: .asciz "\nKernel: You can now safely power down the machine."

##
# Initialize a stack
##
.section .bss
.align 0x04 # 4-byte aligned
.set STACK_SIZE, 0x1000 # 2^12 = 4KB - 1 page
.globl stack
.comm stack, STACK_SIZE

