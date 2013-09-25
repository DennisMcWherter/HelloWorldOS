/**
 * kernel/kernel.c
 *
 * C implementation of kernel methods
 *
 * Author: Dennis J. McWherter, Jr.
 * MIT Lincoln Laboratory
 */

#include "kernel.h"

int kernel_init(int magic, multiboot_info_t* info)
{
  if(magic != MULTIBOOT_COMPLIANT) {
    // TODO: Panic
    return -1;
  }
  return 0;
}

void kernel_main()
{
}

