/**
 * drivers/pic.c
 *
 * Implementation for PIC driver
 *
 * Author: Dennis J. McWherter, Jr.
 * MIT Lincoln Laboratory
 */

#include "pic.h"
#include "../lib/x86_util.h"

int pic_init()
{
  pic_remap(0x20, 0x28);
  return 1;
}

// I always forget this remapping process,
// unfortunately. That said, we are going to
// use the code from: http://wiki.osdev.org/8259_PIC
// to do it. Admittedly, in name of time, I only skimmed
// over the datasheet, BUT I have read it in the past.
void pic_remap(int off1, int off2)
{
  unsigned char a1, a2;

  a1 = inb(PIC1);
  a2 = inb(PIC2);

  outb(PIC1, 0x10+0x01);  // starts the initialization sequence (in cascade mode)
  io_wait();
  outb(PIC2, 0x10+0x01);
  io_wait();
  outb(PIC1_DATA, off1);                 // ICW2: Master PIC vector offset
  io_wait();
  outb(PIC2_DATA, off2);                 // ICW2: Slave PIC vector offset
  io_wait();
  outb(PIC1_DATA, 4);                       // ICW3: tell Master PIC that there is a slave PIC at IRQ2 (0000 0100)
  io_wait();
  outb(PIC2_DATA, 2);                       // ICW3: tell Slave PIC its cascade identity (0000 0010)
  io_wait();

  outb(PIC1_DATA, 0x01);
  io_wait();
  outb(PIC2_DATA, 0x01);
  io_wait();

  outb(PIC1_DATA, a1);   // restore saved masks.
  outb(PIC2_DATA, a2);
}

// This message acknowledges the interrupt
// outb simply sends data to a specified port
void pic_eoi(unsigned char irq)
{
  if(irq >= 8)
    outb(PIC2, PIC_EOI);
  outb(PIC1, PIC_EOI);
}

