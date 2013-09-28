/**
 * kernel/mem/idt.c
 *
 * Interrupt descriptor table
 *
 * Author: Dennis J. McWherter, Jr.
 * MIT Lincoln Laboratory
 */

#include "idt.h"
#include "../../lib/k_stdio.h"

// Assembly method to load idt
extern void idt_load(idt_t*);

// Our data
#define NUM_INTERRUPTS 256
static idt_gate_t idt_e[NUM_INTERRUPTS];
static idt_t idt;

// Other methods
static void set_idt_gate(int num, unsigned offset, unsigned sel, unsigned flags);
void interrupt_handler(); // This can/should accept all the regs pushed on the stack

int idt_init()
{
  idt.limit = (sizeof(idt_gate_t) * NUM_INTERRUPTS) - 1;
  idt.base  = idt_e;

  // Again, this is no joke :/
  set_idt_gate(0, &isr0, 0x08, 0x07);
  set_idt_gate(1, &isr1, 0x08, 0x07);
  set_idt_gate(2, &isr2, 0x08, 0x07);
  set_idt_gate(3, &isr3, 0x08, 0x07);
  set_idt_gate(4, &isr4, 0x08, 0x07);
  set_idt_gate(5, &isr5, 0x08, 0x07);
  set_idt_gate(6, &isr6, 0x08, 0x07);
  set_idt_gate(7, &isr7, 0x08, 0x07);
  set_idt_gate(8, &isr8, 0x08, 0x07);
  set_idt_gate(9, &isr9, 0x08, 0x07);
  set_idt_gate(10, &isr10, 0x08, 0x07);
  set_idt_gate(11, &isr11, 0x08, 0x07);
  set_idt_gate(12, &isr12, 0x08, 0x07);
  set_idt_gate(13, &isr13, 0x08, 0x07);
  set_idt_gate(14, &isr14, 0x08, 0x07);
  set_idt_gate(15, &isr15, 0x08, 0x07);
  set_idt_gate(16, &isr16, 0x08, 0x07);
  set_idt_gate(17, &isr17, 0x08, 0x07);
  set_idt_gate(18, &isr18, 0x08, 0x07);
  set_idt_gate(19, &isr19, 0x08, 0x07);
  set_idt_gate(20, &isr20, 0x08, 0x07);
  set_idt_gate(21, &isr21, 0x08, 0x07);
  set_idt_gate(22, &isr22, 0x08, 0x07);
  set_idt_gate(23, &isr23, 0x08, 0x07);
  set_idt_gate(24, &isr24, 0x08, 0x07);
  set_idt_gate(25, &isr25, 0x08, 0x07);
  set_idt_gate(26, &isr26, 0x08, 0x07);
  set_idt_gate(27, &isr27, 0x08, 0x07);
  set_idt_gate(28, &isr28, 0x08, 0x07);
  set_idt_gate(29, &isr29, 0x08, 0x07);
  set_idt_gate(30, &isr30, 0x08, 0x07);
  set_idt_gate(31, &isr31, 0x08, 0x07);

  idt_load(&idt);
  return 1;
}

// It's a little more "black magic" this time around
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

void interrupt_handler()
{
  k_printf("Interrupt handler");
}

