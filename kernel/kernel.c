/**
 * kernel/kernel.c
 *
 * C implementation of kernel methods
 *
 * Author: Dennis J. McWherter, Jr.
 * MIT Lincoln Laboratory
 */

#include "kernel.h"

// Memory and interrupt
#include <kernel/interrupt/idt.h>
#include <kernel/interrupt/interrupt_handler.h>
#include <kernel/mem/kheap.h>
#include <kernel/mem/gdt.h>
#include <kernel/mem/paging.h>

// lib
#include <lib/k_stdio.h>

// Drivers
#include <drivers/keyboard.h>
#include <drivers/pic.h>
#include <drivers/video.h>

// Global var for the taking
unsigned total_memsize = (1 << 25); // Assume 32MB memory for now

static int init_drivers();
static void gpf_handler(unsigned, registers_t);

int kernel_init(int magic, multiboot_info_t* info)
{
  //multiboot_mmap_t* mmap = 0;
  //unsigned mmap_end = 0;

  if(magic != MULTIBOOT_COMPLIANT) {
    k_panic("GRUB failed to load properly");
    return -1;
  } 

  /** Detect memory */
  /*if(!(info->flags & 0x20)) {
    k_panic("Could not detect memory size");
    return -1;
  }

  // Using code from multiboot spec
   TODO: Eventually detect
  k_printf("Detecting memory...");
  mmap = (multiboot_mmap_t*)(info->mmap_addr + 0xc0000000);
  mmap_end = (unsigned)mmap + info->mmap_length;
  while(mmap < mmap_end) {
    if(mmap->type == 1) { // This means available RAM
      total_memsize += mmap->length_lo;
    }
    mmap = (unsigned)mmap + mmap->size + sizeof(mmap->size);
  }
  k_printf("Done!\n");
  */

  if(!kheap_init()) {
    k_panic("Could not initialize the kernel heap");
    return -1;
  }

  /** Descriptor tables */
  if(!gdt_init()) {
    k_panic("Could not initialize gdt");
    return -1;
  }

  if(!idt_init()) {
    k_panic("Could not initialize idt");
    return -1;
  }

  /** Remaining drivers */
  if(!init_drivers()) {
    k_panic("Could not initialize drivers");
    return -1; // Failed to initialize our drivers
  }

  // Register page fault handler
  register_handler(0xd, gpf_handler);
  register_handler(0xe, page_fault);

  k_success("Detected memory: ");
  k_printf("%d MB\n", total_memsize / 1024 / 1024); // Only 32-bit OS
  k_success("Initialized Kernel Heap...\n");
  k_success("Initialized GDT...\n");
  k_success("Initialized IDT...\n");
  k_success("Initialized Drivers...\n");

  return 0;
}

void kernel_main()
{
  char strint[32];
  k_printf("Sample: %d, %d, 0x%x, 0x%x, 0x%x, %d, %d, 0x%x\n", 10, 111, 10, 0x1337, 0, 0, -50042, 0xdeadbeef);

  // Test interrupts
  __asm__("\tint $0x03\nint $0x04\n");

  keyboard_enable();

  // Try using paging directly :S
  //unsigned page = paging_alloc(0x03);
 // unsigned page2 = paging_alloc(0x03);
 // k_printf("0x%x to 0x%x\n", page, *((unsigned*)page));
  //k_printf("0x%x to 0x%x\n", page2, *((unsigned*)page2));
  //paging_dealloc(page); // Try it.
  //unsigned page3 = paging_alloc(0x03);
  //k_printf("0x%x to 0x%x\n", page, *((unsigned*)page));
  //k_printf("0x%x to 0x%x\n", page3, *((unsigned*)page3));
  //k_itoa(strint, page, 16);
//  k_puts(strint);
}

int init_drivers()
{
  if(!video_init()) {
    k_panic("video_init()\n");
    return 0;
  }
  if(!pic_init()) {
    k_panic("pic_init()\n");
    return 0;
  }
  if(!pit_init()) {
    k_panic("pit_init()\n");
    return 0;
  }
  if(!keyboard_init()) {
    k_panic("keyboard_init()\n");
    // Don't return an error here..
    // Realistically, this may not work
    // if, for instance, a ps/2 keyboard
    // is not present (i.e. my laptop)!
    //return 0;
  }
  return 1;
}

void gpf_handler(unsigned seg, registers_t regs)
{
  k_panic("General Protection Fault:\n");
  k_printf("    EIP: 0x%x, E: 0x%x, Tbl: 0x%x, Idx: 0x%x\n", regs.eip, regs.err & 0x01,
          (regs.err & 0x06) >> 1, (regs.err & 0xffe0) >> 3);
  while(1) { // Hang forever
    __asm__("hlt");
  }
}

