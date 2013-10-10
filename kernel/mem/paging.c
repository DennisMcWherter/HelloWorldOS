/**
 * kernel/mem/paging.c
 *
 * Setup our paging structures, etc.
 *
 * Author: Dennis J. McWherter, Jr.
 * MIT Lincoln Laboratory
 */

#include "paging.h"
#include "kheap.h"

#include <lib/k_stdio.h>
#include <lib/k_string.h>

// These are defined in assembly
extern unsigned KERNEL_VIRT_BADDR;

// We will leave our kernel page directory
// and page tables defined in our reserved kernel
// memory
// We also have a fixed stack
extern unsigned* _page_directory;
extern unsigned** _page_tables;
//extern unsigned _stack;

// Other boundaries
extern unsigned _num_entries;
extern unsigned _lowermem;
extern unsigned _uppermem;

// Defined in kernel.c
extern const unsigned _mem_end_phys;
extern const unsigned total_memsize;

// Book-keeping
#define PAGE_SIZE 4096 // 4096 bytes = 4KB
unsigned num_pages = 0;
// 1024 entries in pde
// 1024 entries per pde so
// 2^5 int's with 2^5 bits per int
// means 1024 status bits in 32 ints.
unsigned page_status[1024][32];
pd_t cur_pd; // Current page directory
unsigned cur_pt; // Current page table

// Bit twiddling macroez
// Oh noez my symbowz...
#define BIT_SET(x, y) (x |= (1 << y))
#define BIT_CLR(x, y) (x &= ~(1 << y))
#define BIT_TEST(x, y) (x & (1 << y))
#define TO_PHYS_ADDR(x) (((unsigned)x) - KERNEL_VIRT_BADDR)
#define TO_VIRT_ADDR(x) (((unsigned)x) + KERNEL_VIRT_BADDR)

int paging_init()
{
  unsigned i = 0, j = 0;
  unsigned ident_addr = 0;
  unsigned kernel_pde_off = (KERNEL_VIRT_BADDR >> 22);

  // Convert to physical addresses for this method
  unsigned* np_addr  = (unsigned*)TO_PHYS_ADDR(&num_pages);
  unsigned* pst_phys = (unsigned*)TO_PHYS_ADDR(page_status);//(unsigned*)(((unsigned)page_status) - KERNEL_VIRT_BADDR);
  pd_t* pd_addr  = (pd_t*)TO_PHYS_ADDR(&cur_pd);//(unsigned*)(((unsigned)&cur_pd) - KERNEL_VIRT_BADDR);  
  unsigned* pt_addr  = (unsigned*)TO_PHYS_ADDR(&cur_pt);//(unsigned*)(((unsigned)&cur_pt) - KERNEL_VIRT_BADDR);  

  // Set all pages to unused.
  k_memset(pst_phys, 0, 1024*32);

  // Perhaps this is ludacris... But let's
  // begin by identity mapping ALL THE THINGS!
  //
  // NOTE: Just because these are identity mapped, doesn't
  //    guarantee success on access if the address is
  //    greater than the size of RAM installed!
  //
  // We'll also map higher half here :) no reason to do the loop twice!
  for(i = 0 ; i < _lowermem ; ++i) {
    // Identity map
    _page_directory[i] = (unsigned)(&_page_tables[i*1024]) | 0x03;
    // Higher half map  
    _page_directory[kernel_pde_off + i] = _page_directory[i];

    // Just set the bit!
//    ((unsigned**)pst_phys)[kernel_pde_off][i / 32] |= (1 << (i % 32));
    BIT_SET(((unsigned**)pst_phys)[kernel_pde_off][i/32], (i%32));
    
    // Populate table
    ident_addr  = (i << 22); // High part of address
    ident_addr |= 0x03; // Control bits
    for(j = 0 ; j < _num_entries ; ++j) {
      ident_addr &= 0xfffc00fff; // Clear middle 10 bits
      ident_addr |= (j << 12); // Set the appropriate value
      *(_page_tables+(i*1024)+j) = ident_addr; 
    }
  }

  // Determine the number of pages
  // available for us to map -- btw, we're still in physical address mode!!
  *np_addr = (total_memsize - _mem_end_phys) / PAGE_SIZE;

  // Final book-keeping setup
  pd_addr->page_dir = (unsigned*)TO_VIRT_ADDR(_page_directory);
  pd_addr->page_status = page_status; 
  pd_addr->phys_addr = _page_directory;
  *pt_addr = (unsigned**)TO_VIRT_ADDR(_page_tables);

  return 1;
}

// Naive implementation for simplicity!
unsigned paging_next_frame()
{
  // First find our available next page
  int i = 0, j = 0, k = 0;
  void* ret = 0;

  // Check frames
  // Naive approach
  for(i = 0 ; i < 1024 ; ++i) {
    for(j = 0 ; j < 32 ; ++j) {
      for(k = 0 ; k < 32 ; ++k) {

        if(!BIT_TEST(cur_pd.page_status[i][j], (1 << k))) {
          // use this page
          BIT_SET(cur_pd.page_status[i][j], (1 << k));
        }
      }
    }
  }

  if(i == 1024 && j == 32 && k == 32) {
    k_panic("Out of memory!\n");
    return 0;
  }

  return 0;
}

void page_fault(unsigned not_used, registers_t regs)
{
  k_printf("Page fault!\n");
}

