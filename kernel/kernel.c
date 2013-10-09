/**
 * kernel/kernel.c
 *
 * C implementation of kernel methods
 *
 * Author: Dennis J. McWherter, Jr.
 * MIT Lincoln Laboratory
 */

#include "kernel.h"

#include <kernel/mem/paging.h>
#include <kernel/mem/gdt.h>
#include <kernel/interrupt/idt.h>

// lib
#include <lib/k_stdio.h>

// Drivers
#include <drivers/pic.h>
#include <drivers/video.h>

// Global var for the taking
unsigned total_memsize = (1 << 25); // Assume 32MB memory for now

static int init_drivers();

int kernel_init(int magic, multiboot_info_t* info)
{
  multiboot_mmap_t* mmap = 0;

  if(magic != MULTIBOOT_COMPLIANT) {
    k_panic("GRUB failed to load properly");
    return -1;
  } 

  /** Detect memory */
  if(!(info->flags & 0x06)) {
    k_panic("Could not detect memory size");
    return -1;
  }

  // Using code from multiboot spec
  /*
  // TODO: Memory detection
  for(mmap = (multiboot_mmap_t*)info->mmap_addr
    ; mmap < info->mmap_addr + info->mmap_length 
    ; mmap = mmap + mmap->size + sizeof(mmap->size)) {
    total_memsize += mmap->size;
  }
  */

  total_memsize = mmap->length_lo;

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

  k_success("Detected memory: ");
  k_printf("0x%x bytes\n", total_memsize); // Only 32-bit OS
  k_success("Initialized GDT...\n");
  k_success("Initialized IDT...\n");
  k_success("Initialized Drivers...\n");

  return 0;
}

void kernel_main()
{
  k_printf("Sample: %d, %d, 0x%x, 0x%x, 0x%x, %d, %d\n", 10, 111, 10, 0x1337, 0, 0, -50042);
  // Test interrupts
  __asm__("\tint $0x03\nint $0x04\n");
}

int init_drivers()
{
  if(!video_init()) {
    k_panic("video_init()");
    return 0;
  }
  if(!pic_init()) {
    k_panic("pic_init()");
    return 0;
  }
  return 1;
}

