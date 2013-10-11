##
# x86/idt.s
#
# x86 code for IDT. Mostly very simple
# code, but plenty of duplication (this is how linux
# deals with this problem too).
#
# Author: Dennis J. McWherter, Jr.
# MIT Lincoln Laboratory
##
.intel_syntax noprefix

.extern interrupt_handler

.macro isr num
.globl isr\num
isr\num:
  cli # Disable interrupts while we handle
  push 0
  push \num
  jmp isr_handler # Jump is important! We want same stackframe
.endm

.macro isr_err num
.globl isr\num
isr\num:
  cli
  push \num
  jmp isr_handler
.endm

# void idt_load(idt_t*);
.globl idt_load
idt_load:
  mov eax, [esp+4]
  lidt [eax]
  ret

isr_handler:
  # Save our registers
  # Push EAX, ECX, EDX, EBX, original ESP, EBP, ESI, and EDI
  pusha

  mov ax, ds
  push eax

  # Load kernel segment
  mov eax, 0x10
  mov ds, ax
  mov es, ax
  mov fs, ax
  mov gs, ax

  # Our interrupt handler (defined in C)
  call interrupt_handler

  # Restore state
  pop ecx # We don't want to trash our return value
  mov ds, cx
  mov es, cx
  mov fs, cx
  mov gs, cx

  popa # Pop the registers we saved
  add esp, 0x08 # Pop the intno & err arg
  sti # Re-enable interrupts
  iret # Return from interrupt

###
# Now for teh macroez
##

isr 0
isr 1
isr 2
isr 3
isr 4
isr 5
isr 6
isr 7
isr_err 8
isr 9
isr_err 10
isr_err 11
isr_err 12
isr_err 13
isr_err 14
isr 15
isr 16
isr 17
isr 18
isr 19
isr 20
isr 21
isr 22
isr 23
isr 24
isr 25
isr 26
isr 27
isr 28
isr 29
isr 30
isr 31
isr 32
isr 33
isr 34
isr 35
isr 36

