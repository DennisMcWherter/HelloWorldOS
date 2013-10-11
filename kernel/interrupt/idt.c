/**
 * kernel/interrupt/idt.c
 *
 * Interrupt descriptor table
 *
 * Author: Dennis J. McWherter, Jr.
 * MIT Lincoln Laboratory
 */

#include "idt.h"

// Assembly method to load idt
extern void idt_load(idt_t*);
extern int KERNEL_VIRT_BADDR;

// Our data
#define NUM_INTERRUPTS 256
static idt_gate_t idt_e[NUM_INTERRUPTS];
static idt_t idt;

// Other methods
static void set_idt_gate(int num, unsigned offset, unsigned sel, unsigned flags);

int idt_init()
{
  idt.limit = (sizeof(idt_gate_t) * NUM_INTERRUPTS) - 1;
  idt.base  = idt_e;

  // Again, this is no joke :/
  set_idt_gate(0,  (unsigned)&isr0 + KERNEL_VIRT_BADDR, 0x08, 0x07);
  set_idt_gate(1,  (unsigned)&isr1 + KERNEL_VIRT_BADDR, 0x08, 0x07);
  set_idt_gate(2,  (unsigned)&isr2 + KERNEL_VIRT_BADDR, 0x08, 0x07);
  set_idt_gate(3,  (unsigned)&isr3 + KERNEL_VIRT_BADDR, 0x08, 0x07);
  set_idt_gate(4,  (unsigned)&isr4 + KERNEL_VIRT_BADDR, 0x08, 0x07);
  set_idt_gate(5,  (unsigned)&isr5 + KERNEL_VIRT_BADDR, 0x08, 0x07);
  set_idt_gate(6,  (unsigned)&isr6 + KERNEL_VIRT_BADDR, 0x08, 0x07);
  set_idt_gate(7,  (unsigned)&isr7 + KERNEL_VIRT_BADDR, 0x08, 0x07);
  set_idt_gate(8,  (unsigned)&isr8 + KERNEL_VIRT_BADDR, 0x08, 0x07);
  set_idt_gate(9,  (unsigned)&isr9 + KERNEL_VIRT_BADDR, 0x08, 0x07);
  set_idt_gate(10, (unsigned)&isr10 + KERNEL_VIRT_BADDR, 0x08, 0x07);
  set_idt_gate(11, (unsigned)&isr11 + KERNEL_VIRT_BADDR, 0x08, 0x07);
  set_idt_gate(12, (unsigned)&isr12 + KERNEL_VIRT_BADDR, 0x08, 0x07);
  set_idt_gate(13, (unsigned)&isr13 + KERNEL_VIRT_BADDR, 0x08, 0x07);
  set_idt_gate(14, (unsigned)&isr14 + KERNEL_VIRT_BADDR, 0x08, 0x07);
  set_idt_gate(15, (unsigned)&isr15 + KERNEL_VIRT_BADDR, 0x08, 0x07);
  set_idt_gate(16, (unsigned)&isr16 + KERNEL_VIRT_BADDR, 0x08, 0x07);
  set_idt_gate(17, (unsigned)&isr17 + KERNEL_VIRT_BADDR, 0x08, 0x07);
  set_idt_gate(18, (unsigned)&isr18 + KERNEL_VIRT_BADDR, 0x08, 0x07);
  set_idt_gate(19, (unsigned)&isr19 + KERNEL_VIRT_BADDR, 0x08, 0x07);
  set_idt_gate(20, (unsigned)&isr20 + KERNEL_VIRT_BADDR, 0x08, 0x07);
  set_idt_gate(21, (unsigned)&isr21 + KERNEL_VIRT_BADDR, 0x08, 0x07);
  set_idt_gate(22, (unsigned)&isr22 + KERNEL_VIRT_BADDR, 0x08, 0x07);
  set_idt_gate(23, (unsigned)&isr23 + KERNEL_VIRT_BADDR, 0x08, 0x07);
  set_idt_gate(24, (unsigned)&isr24 + KERNEL_VIRT_BADDR, 0x08, 0x07);
  set_idt_gate(25, (unsigned)&isr25 + KERNEL_VIRT_BADDR, 0x08, 0x07);
  set_idt_gate(26, (unsigned)&isr26 + KERNEL_VIRT_BADDR, 0x08, 0x07);
  set_idt_gate(27, (unsigned)&isr27 + KERNEL_VIRT_BADDR, 0x08, 0x07);
  set_idt_gate(28, (unsigned)&isr28 + KERNEL_VIRT_BADDR, 0x08, 0x07);
  set_idt_gate(29, (unsigned)&isr29 + KERNEL_VIRT_BADDR, 0x08, 0x07);
  set_idt_gate(30, (unsigned)&isr30 + KERNEL_VIRT_BADDR, 0x08, 0x07);
  set_idt_gate(31, (unsigned)&isr31 + KERNEL_VIRT_BADDR, 0x08, 0x07);
  set_idt_gate(32, (unsigned)&isr32 + KERNEL_VIRT_BADDR, 0x08, 0x07);
  set_idt_gate(33, (unsigned)&isr33 + KERNEL_VIRT_BADDR, 0x08, 0x07);
  set_idt_gate(34, (unsigned)&isr34 + KERNEL_VIRT_BADDR, 0x08, 0x07);
  set_idt_gate(35, (unsigned)&isr35 + KERNEL_VIRT_BADDR, 0x08, 0x07);
  set_idt_gate(36, (unsigned)&isr36 + KERNEL_VIRT_BADDR, 0x08, 0x07);

  idt_load(&idt);
  return 1;
}

// I don't show the bits quite as explicitly this time around
// but I figured you should have the gist of this
// from the GDT one
// Same concept, different structure
// NOTICE: This method can be more generic to accomodate
//   other gate types. We are being a little stingy right now
void set_idt_gate(int num, unsigned offset, unsigned sel, unsigned flags)
{
  idt_gate_t* e = &idt_e[num];

  e->offset_lo = offset & 0xffff;
  e->offset_hi = (offset & 0xffff0000) >> 16;

  e->selector = sel & 0xffff;

  // Constants
  e->reserved = 0;
  e->zero  = 0;
  e->zero2 = 0;
  e->zero3 = 0;
  e->one   = 0x07;

  // Our encoding for flags
  // Bit 0 = p
  // Bit 1-2 = dpl
  e->p   = flags & 0x01;
  e->dpl = (flags & 0x06) >> 1;
}


