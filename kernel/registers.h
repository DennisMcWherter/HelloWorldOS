/**
 * kernel/registers.h
 *
 * Struct to define structure of registers passed
 * in as a variable
 *
 * Author: Dennis J. McWherter, Jr.
 * MIT Lincoln Laboratory
 */

#ifndef KERNEL_REGISTERS_H__
#define KERNEL_REGISTERS_H__

/**
 * Structure to hold our registers after a "pusha"
 * Refer to Intel Manual 
 */
typedef struct
{
  unsigned edi, esi, ebp, esp;
  unsigned ebx, edx, ecx, eax;
} __attribute__((__packed__)) registers_t;

#endif /** KERNEL_REGISTERS_H__ */

