/**
 * kernel/kernel.h
 *
 * Header to define important kernel structs 
 *
 * Author: Dennis J. McWherter, Jr.
 * MIT Lincoln Laboratory
 */

#ifndef KERNEL_KERNEL_H__
#define KERNEL_KERNEL_H__

// If we were booted properly by the bootloader,
// this should be the value
#define MULTIBOOT_COMPLIANT 0x2badb002

/**
 * Based on spec:
 * http://www.gnu.org/software/grub/manual/multiboot/multiboot.html#Machine-state
 */
typedef struct
{
  unsigned flags;
  unsigned mem_lower : 16, mem_upper : 16;
  unsigned short boot_dev;
  unsigned cmdline;
  unsigned mods_count;
  unsigned mods_addr;
  unsigned syms[3];
  unsigned mmap_length;
  unsigned mmap_addr;
  unsigned drives_length;
  unsigned drives_addr;
  unsigned config_table;
  unsigned char boot_loader_name[4];
  unsigned apm_table;
  unsigned vbe_control_info;
  unsigned vbe_mode_info;
  unsigned short vbe_mode;
  unsigned short vbe_interface_seg, vbe_interface_off, vbe_interface_len;
} __attribute__((__packed__)) multiboot_info_t;

typedef struct
{
  unsigned size;
  unsigned base_lo, base_hi;
  unsigned length_lo, length_hi;
  unsigned type;
} __attribute__((__packed__)) multiboot_mmap_t;

#endif /** KERNEL_KERNEL_H__ */

