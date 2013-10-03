/**
 * drivers/pic.h
 *
 * Programmable Interrupt Controller (PIC) driver
 * I think typically, APICs are more common now
 * but since we are single core/mostly VM-running
 * OS, we will program PIC
 *
 * Useful for distinguishing between interrupts and
 * exceptions.
 *
 * Useful datasheet: http://pdos.csail.mit.edu/6.828/2005/readings/hardware/8259A.pdf
 * This code: http://wiki.osdev.org/8259_PIC
 *
 * Author: Dennis J. McWherter, Jr.
 * MIT Lincoln Laboratory
 */

#ifndef DRIVERS_PIC_H__
#define DRIVERS_PIC_H__

#define PIC1 0x20
#define PIC2 0xa0
#define PIC1_DATA 0x21
#define PIC2_DATA 0xa1
#define PIC_EOI 0x20

int pic_init();

void pic_remap(int off1, int off2);
void pic_eoi(unsigned char irq);

#endif /** DRIVERS_PIC_H__ */

