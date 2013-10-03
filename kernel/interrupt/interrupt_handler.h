/**
 * kernel/interrupt/interrupt_handler.h
 *
 * Interrupt handler interface
 *
 * Author: Dennis J. McWherter, Jr.
 * MIT Lincoln Laboratory
 */

#ifndef KERNEL_INTERRUPT_INTERRUPT_HANDLER_H__
#define KERNEL_INTERRUPT_INTERRUPT_HANDLER_H__

#include <kernel/registers.h>

typedef void(*isr_func_t)(unsigned,registers_t);

int register_handler(int,isr_func_t);
void unregister_handler(int);

#endif /** KERNEL_INTERRUPT_INTERRUPT_HANDLER_H__ */

