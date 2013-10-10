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

/**
 * Prototype for interrupt handlers
 */
typedef void(*isr_func_t)(unsigned,registers_t);

/**
 * Register an interrupt handler
 *
 * @param int   Interrupt number
 * @param isr   Function pointer to handler
 * @return  1 on success 0 otherwise
 */ 
int register_handler(int,isr_func_t);

/**
 * Unregister an interrupt handler
 *
 * @param int   Interrupt number
 */
void unregister_handler(int);

#endif /** KERNEL_INTERRUPT_INTERRUPT_HANDLER_H__ */

