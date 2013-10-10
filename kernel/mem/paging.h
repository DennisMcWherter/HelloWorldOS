/**
 * kernel/mem/paging.h
 *
 * Paging interface. Allows for
 * allocation, etc. Enabling this
 * takes place in the boot routine (x86)!
 *
 * Author: Dennis J. McWherter, Jr.
 * MIT Lincoln Laboratory
 */

#ifndef KERNEL_MEM_PAGING_H__
#define KERNEL_MEM_PAGING_H__

#include <kernel/registers.h>

/**
 * Struct to hold information about
 * page directory
 *
 * NOTE: Physical address is actually
 *       important to keep track of
 *       for future reference. If we
 *       have processes and want to switch
 *       page directories, we will need to
 *       set the phys addr to the pd in the cpu
 */
typedef struct
{
  unsigned* page_dir; // Pointer to directory
  unsigned** page_status; // Pointer to status bitmap 
  unsigned phys_addr; // Physical address
} __attribute__((__packed__)) pd_t;

/**
 * Initialize paging
 *
 * @return  1 on success, false otherwise
 */
int paging_init();

/**
 * Retrieve the next frame from the current
 * page table
 *
 * @return  Address to next frame
 */
unsigned paging_next_frame();

// TODO: Implement method to switch page tables
//       for processes

// Our page fault handler
void page_fault(unsigned, registers_t);

#endif /** KERNEL_MEM_PAGING_H__ */

