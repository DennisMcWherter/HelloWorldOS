/**
 * kernel/interrupt/interrupt_handler.c
 *
 * Main interrupt handler
 *
 * Author: Dennis J. McWherter, Jr.
 * MIT Lincoln Laboratory
 */

#include "interrupt_handler.h"

#include <drivers/pic.h>
#include <lib/k_stdio.h>

#define NUM_INTERRUPTS 256
isr_func_t handler[NUM_INTERRUPTS] = { 0 };

void interrupt_handler(unsigned data_seg, registers_t regs, unsigned intno)
{
  if(handler[intno] != 0) {
    handler[intno](data_seg, regs);
  } else {
    k_printf("Unhandled interrupt: %d\n", intno);
  }
  pic_eoi(intno);
}

// Returns 1 on success 0 otherwise
int register_handler(int no, isr_func_t func)
{
  if(no < 0 || no >= NUM_INTERRUPTS)
    return 0;
  handler[no] = func;
  return 1;
}

void unregister_handler(int no)
{
  if(no < 0 || no >= NUM_INTERRUPTS)
    return;
  handler[no] = 0;
}

