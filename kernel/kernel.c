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
#include <drivers/video.h>

static int init_drivers();

int kernel_init(int magic, multiboot_info_t* info)
{
  if(magic != MULTIBOOT_COMPLIANT) {
    // Honestly, we would probably crash in k_panic
    // since we don't map the portion of the stack
    // that the string is on.
    k_panic("GRUB failed to load properly");
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

