/**
 * kernel/interrupt/idt.h
 *
 * Interrupt descriptor table header
 *
 * Author: Dennis J. McWherter, Jr.
 * MIT Lincoln Laboratory
 */

#ifndef KERNEL_INTERRUPT_IDT_H__
#define KERNEL_INTERRUPT_IDT_H__

#include "../segment_descriptor.h"

/**
 * IDT gate
 * Review Intel Manual Figure 6-2
 * for more information
 */
typedef struct
{
  unsigned short offset_lo;
  unsigned short selector;
  unsigned char reserved : 5, zero : 3;
  unsigned char zero2 : 1, one : 3, zero3 : 1, dpl : 2, p : 1;
  unsigned short offset_hi;
} __attribute__((__packed__)) idt_gate_t;

/**
 * Interrupt descriptor table
 * Refer to Intel Manual section 6.10
 * and figure 6-2 for setting up each entry
 *
 * Table structure is the same as GDT
 */
typedef struct
{
  unsigned short limit;
  unsigned base;
} __attribute__((__packed__)) idt_t;

// Methods
int idt_init();

// No, sadly, this is not a joke :(
extern void isr0();
extern void isr1();
extern void isr2();
extern void isr3();
extern void isr4();
extern void isr5();
extern void isr6();
extern void isr7();
extern void isr8();
extern void isr9();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr15();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();
extern void isr32();
extern void isr33();
extern void isr34();
extern void isr35();
extern void isr36();

#endif /** KERNEL_INTERRUPT_IDT_H__ */

