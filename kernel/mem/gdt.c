/**
 * kernel/mem/gdt.c
 *
 * gdt initialization
 *
 * Author: Dennis J. McWherter, Jr.
 * MIT Lincoln Laboratory
 */

#include "gdt.h"
#include "../../lib/k_string.h"

// Our method to setup descriptors
static void setup_desc(int num, unsigned limit, unsigned base, unsigned type, unsigned s, unsigned dpl, unsigned p, unsigned a, unsigned g);

// Assembly method to actually load the gdt into memory
extern void gdt_load(gdt_t*);

// Our global descriptor table
#define NUM_DESCRIPTORS 6
static segment_desc_t segs[NUM_DESCRIPTORS];
static tss_desc_t tss;
static gdt_t gdt;

int gdt_init()
{
  // Setup the gdt
  gdt.limit = (sizeof(segment_desc_t) * NUM_DESCRIPTORS) - 1;
  gdt.base  = segs;

  // Initialize all the descriptors properly
  // For properly setting the type field, please
  // refer to Table 3-1 in the Intel Manual
  // The magic numbers you will see come from 
  // that table (3 bits, not including S since
  // we have an explicit param for it)
  setup_desc(0, 0, 0, 0, 0, 0, 0, 0, 0); // Null descriptor

  // I'll explain this once:
  // param0: 1 = descriptor number we are setting
  // param1: 0xffffffff = Size of segment. Use full addr space
  // param2: 0 = Base of the memory segment. Start at 0x00
  // param3: 0x0a = Binary 1010 makes segment read/executable
  // param4: 1 = code/data
  // param5: 0 = Segment executes in ring 0 (kernel seg)
  // param6: 1 = present/in-use/don't overwrite me!
  // param7: 0 = This bit is free for us to use how we want
  // param8: 1 = 4KB seg chunks
  setup_desc(1, 0xffffffff, 0, 0x0a, 1, 0, 1, 0, 1); // Kernel code seg
  setup_desc(2, 0xffffffff, 0, 0x03, 1, 0, 1, 0, 1); // Kernel data
  setup_desc(3, 0xffffffff, 0, 0x0a, 1, 3, 1, 0, 1); // User code
  setup_desc(4, 0xffffffff, 0, 0x03, 1, 3, 1, 0, 1); // User data 

  // Refer to Intel Manual Figure 7-3 for information
  // on setting up TSS descriptor
  setup_desc(5, sizeof(tss), (unsigned)&tss, 0x09, 0, 0, 1, 0, 0);

  // Just clear tss for now
  k_memset(&tss, 0, sizeof(tss));

  // Load the gdt
  gdt_load(&gdt);

  return 1;
}

// This method is nothing but fancy bit-twiddling
// In practice, I would likely suggest combining mostly
// all of these parameters except num, limit, and base.
// Another int could, realistically, cover the rest of
// the parameters if you properly encode them in it.
void setup_desc(int num, unsigned limit, unsigned base, unsigned type, unsigned s, unsigned dpl, unsigned p, unsigned a, unsigned g)
{
  segment_desc_t* seg = &segs[num];

  // Set limit
  seg->limit_lo = limit & 0xffff;
  seg->limit_hi = (limit & 0xffff0000) >> 16;

  // Set base
  seg->base_lo  = base & 0xffff;
  seg->base_mid = (base & 0x00ff0000) >> 16;
  seg->base_hi  = (base & 0xff000000) >> 24;

  // Constant values
  seg->zero = 0;
  seg->one  = 1;

  // Remaining options
  seg->type  = type & 0x0f;
  seg->s     = s & 0x01;
  seg->avail = a & 0x01;
  seg->dpl   = dpl & 0x03;
  seg->p     = p & 0x01;
  seg->g     = g & 0x01;
}


