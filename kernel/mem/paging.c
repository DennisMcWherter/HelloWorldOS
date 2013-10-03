/**
 * kernel/mem/paging.c
 *
 * Setup our paging structures, etc.
 *
 * Author: Dennis J. McWherter, Jr.
 * MIT Lincoln Laboratory
 */

#include "paging.h"

extern unsigned KERNEL_VIRT_BADDR;

extern unsigned* _page_directory;
extern unsigned** _page_tables;
extern unsigned _stack;

extern unsigned _num_entries;
extern unsigned _lowermem;
extern unsigned _uppermem;

int paging_init()
{
  unsigned i = 0, j = 0;
  unsigned ident_addr = 0;
  unsigned kernel_pde_off = (KERNEL_VIRT_BADDR >> 22);

  /** Perhaps this is ludacris... But let's
    begin by identity mapping ALL THE THINGS!
    (up to 1GB of address) */
  for(i = 0 ; i < _lowermem ; ++i) {
    // Identity map
    _page_directory[i] = (unsigned)(&_page_tables[i*1024]) | 0x03;
    // Higher half map  
    _page_directory[kernel_pde_off + i] = _page_directory[i];
    
    // Populate table
    ident_addr  = (i << 22); // High part of address
    ident_addr |= 0x03; // Control bits
    for(j = 0 ; j < _num_entries ; ++j) {
      ident_addr &= 0xfffc00fff; // Clear middle 10 bits
      ident_addr |= (j << 12); // Set the appropriate value
      *(_page_tables+(i*1024)+j) = ident_addr; 
    }
  }

  return 1;
}

