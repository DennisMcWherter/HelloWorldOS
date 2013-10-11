/**
 * kernel/mem/paging.c
 *
 * Setup our paging structures, etc.
 *
 * Author: Dennis J. McWherter, Jr.
 * MIT Lincoln Laboratory
 */

#include "paging.h"
#include "kheap.h" // Goal was to use this to support our
                   // page allocation and then create a higher
                   // level heap manager depending on pages
                   // to provide better granularity. That said,
                   // busy week-- ran out of time. Sorry!

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
// Naive way-- this is also wasteful!
unsigned num_pages = 0;
// 1024 entries in pde
// 1024 entries per pde so
// 2^5 int's with 2^5 bits per int
// means 1024 status bits in 32 ints.
unsigned page_status[1024][32];
unsigned phys_mem[1024][32]; // This many 4KB Pages
pd_t cur_pd; // Current page directory

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
  unsigned* pm_addr  = (unsigned*)TO_PHYS_ADDR(phys_mem);
  pd_t* pd_addr  = (pd_t*)TO_PHYS_ADDR(&cur_pd);//(unsigned*)(((unsigned)&cur_pd) - KERNEL_VIRT_BADDR);  

  // Set all pages to unused.
  k_memset(pst_phys, 0, 1024*32*sizeof(unsigned));
  k_memset(pm_addr, 0, 1024*32*sizeof(unsigned));

  // Treat physical kernel memory as static
  // HACK: Should actually go through.. This is wasteful
  k_memset(pm_addr, 0xffffffff, (_mem_end_phys / 32) + 1);

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
    for(j = 0 ; j < 1024 ; ++j) {
      ident_addr &= 0xfffc00fff; // Clear middle 10 bits
      ident_addr |= (j << 12); // Set the appropriate value
      *(_page_tables+(i*1024)+j) = ident_addr; 
    }
  }

  // Determine the number of pages
  // available for us to map -- btw, we're still in physical address mode!!
  *np_addr = (total_memsize - _mem_end_phys) / PAGE_SIZE;

  // Final book-keeping setup
  pd_addr->page_dir         = (unsigned*)TO_VIRT_ADDR(_page_directory);
  pd_addr->status           = page_status; 
  pd_addr->page_tables      = (unsigned*)TO_VIRT_ADDR(_page_tables); 
  pd_addr->page_tables_phys = (unsigned*)_page_tables;
  pd_addr->phys_addr        = _page_directory;

  return 1;
}

// NOTE: The rest of this code is broken. Don't use it.
//       I suppose that's what happens when you work on code
//       at 3am -.-

// Naive implementation for simplicity!
unsigned paging_alloc(int control)
{
  // First find our available next page
  int i = 0, j = 0, k = 0;
  int pi = 0, pj = 0, pk = 0; // State in bitmap
  int phi = 0, phj = 0, phk = 0; // Other state
  int in_use = 0;
  unsigned* page = 0;
  unsigned phys_addr = 0;

  // Find an available page
  for(i = 0 ; i < 1024 ; ++i) {
    for(j = 0 ; j < 32 ; ++j) {
      for(k = 0 ; k < 32 ; ++k) { 
        if(page == 0) {
          in_use = BIT_TEST(cur_pd.status[i][j], k); 
          if(!in_use) {
            page = (unsigned)cur_pd.page_tables + i*1024 + j*k + k;
            pi = i;
            pj = j;
            pk = k;
            break;
          }
        }

        if(phys_addr == 0) {
          in_use = BIT_TEST(phys_mem[i][j], k);
          if(!in_use) {
            phi = i;
            phj = j;
            phk = k;
            // Bit of a hack for page alignment-- should verify later we are not overwriting something important
            phys_addr = (((unsigned)_mem_end_phys + (i + j*k + k)*PAGE_SIZE) & 0xfffff000) | control;
          }
        }

        if(phys_addr != 0 && page != 0) { // Found, quit early
          break;
        }
      }
    }
  }

  if(phys_addr == 0) {
    return 0;
  }

  // By our setup we assume pages are already alloc'ed and mapped
  // this is _NOT_ a good general assumption and would need to
  // be changed to support processes/multiple address spaces
  for(i = 0 ; i < 1024 ; ++i) {
      unsigned* table = (unsigned*)cur_pd.page_dir[i];
    for(j = 0 ; j < 1024 ; ++j) {
      if(!BIT_TEST(table[j], 1)) { // Not present
        table[j]  = phys_addr;
        table[j] |= control;
        table[j] |= 1; // Make sure we set the present bit 
        pi = i;
        pj = j;
        break;
      }
    }
  }

#if 0
  // Find physical slice
  for(i = 0 ; i < (1024*32) ; ++i) {
    for(j = 0 ; j < 32 ; ++j) {
      in_use = BIT_TEST(phys_mem[i][j], k);
      if(!in_use) {
        phi = i;
        phj = j;
        phys_addr  = (unsigned)_mem_end_phys + (32*i + j)*PAGE_SIZE; // Move at 4KB regions
        phys_addr |= control;
      }
    }
  }
#endif

  if(page != 0 && phys_addr != 0) { // Setup page with physical address
    // Claim this memory for ourselves
    BIT_SET(cur_pd.status[pi][pj], pk);
    BIT_SET(phys_mem[phi][phj], phk);
    *page = phys_addr;
    // Update page directory
    cur_pd.page_dir[i] = ((unsigned)cur_pd.page_tables_phys + pi*1024) | control;
  }

  //return page;
  return (pi << 22) | (pj << 12);
}

// This method does not error check and it probably should.
void paging_dealloc(unsigned* page)
{
  // Set physical memory to free
  unsigned page_off = (unsigned)page - (unsigned)cur_pd.page_tables;
  unsigned phys_off = (*page - _mem_end_phys) / PAGE_SIZE;
  unsigned i = 0, j = 0, k = 0;

  // Calculate i and j index for physical address
  k = phys_off % 32;
  j = (phys_off - k) % (1024);
  i = (phys_off - j - k) / 1024; // Remaining is the i index

  BIT_CLR(phys_mem[i][j], k);

  // Calculate indices for page
  k = page_off % 32;
  j = (page_off - k) % (1024);
  i = (page_off - j - k) / 1024; // Remaining is the i index

  BIT_CLR(cur_pd.page_tables[i][j], k);

  // Update the page directory
  cur_pd.page_dir[i] = 0x0;

  // Unmap
  *page = 0;
}

void page_fault(unsigned not_used, registers_t regs)
{
  k_printf("Page fault!\n");
}

