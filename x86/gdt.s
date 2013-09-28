##
# x86/gdt.s
#
# Assembly source to load the GDT
#
# Author: Dennis J. McWherter, Jr.
# MIT Lincoln Laboratory
##
.intel_syntax noprefix

# void gdt_load(gdt_t*)
.globl gdt_load
gdt_load:
  mov eax, [esp+4]
  lgdt [eax]

  mov ax, 0x10 # To data segment
  mov ds, ax
  mov es, ax
  mov fs, ax
  mov gs, ax
  mov ss, ax

  jmp 0x08:.finish

.finish:
  ret

