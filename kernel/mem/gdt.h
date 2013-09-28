/**
 * kernel/mem/gdt.h
 *
 * Header for important gdt structures
 * and interface to initialization call
 *
 * Author: Dennis J. McWherter, Jr.
 * MIT Lincoln Laboratory
 */

#ifndef KERNEL_MEM_GDT_H__
#define KERNEL_MEM_GDT_H__

#include "segment_descriptor.h"

/**
 * Global descriptor table
 * Review Intel Manual section 3.5.1
 * Particularly, figures 2-6 & 3-11
 */
typedef struct
{
  unsigned short limit; // Table size
  segment_desc_t* base; // Pointer to the table of entries
} __attribute__((__packed__)) gdt_t;

// Method to initialize the global descriptor table
int gdt_init();

#endif /** KERNEL_MEM_GDT_H__ */

