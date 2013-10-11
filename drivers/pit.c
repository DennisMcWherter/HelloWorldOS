/**
 * drivers/pit.c
 *
 * PIT Implementation
 *
 * Author: Dennis J. McWherter, Jr.
 * MIT Lincoln Laboratory
 */

#include "pit.h"

#include <kernel/interrupt/interrupt_handler.h>

static void pit_handler(unsigned, registers_t);

int pit_init()
{
  register_handler(0x20, pit_handler);
  return 1;
}

void pit_handler(unsigned seg, registers_t regs)
{
  // Delibrately and silently unhandled right now
  // this can be useful to implement sleep() functionality
  // or process preemption
}

