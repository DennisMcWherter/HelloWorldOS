/**
 * kernel/mem/kheap.c
 *
 * Implementation of kernel heap
 *
 * Author: Dennis J. McWherter, Jr.
 */

#include "kheap.h"

// Necessary vars
extern unsigned _mem_end_phys; // Remember this from the linker script?
extern unsigned total_memsize; // Defined in kernel.c

// Internal state
static unsigned cur_addr = 0;

int kheap_init()
{
  kheap_free_all();
  return 1;
}

void* kheap_malloc(int size)
{
  return kheap_malloc_pl(size, cur_addr);
}

void* kheap_malloc_pl(int size, unsigned pl)
{
  unsigned addr = 0;
  if((pl + size) < total_memsize) {
    cur_addr = addr = pl;
    cur_addr += size;
  }
  return (void*)addr;
}

void kheap_free_all()
{
  // It just resets
  cur_addr = _mem_end_phys;
}

