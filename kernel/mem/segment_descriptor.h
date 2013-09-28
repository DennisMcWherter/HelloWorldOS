/**
 * kernel/mem/segment_descriptor.h
 *
 * Struct describing a segment descriptor
 * Refer to Intel Manual section 3.4.5
 * Figure 3-8 & 3-10
 *
 * Author: Dennis J. McWherter, Jr.
 * MIT Lincoln Laboratory
 */

#ifndef KERNEL_MEM_SEGMENT_DESCRIPTOR_H__
#define KERNEL_MEM_SEGMENT_DESCRIPTOR_H__

/**
 * Segment descriptor structure
 *  Base address is setup interestingly
 *  due to back-compat issues
 *
 * NOTE: I have used bitfields for
 *    readability, but it is not
 *    strictly necessary.
 *
 * REMEMBER: This struct is nothing more than bits.
 *        You can do the _EXACT_ samething by making
 *        this a char[8] and manually setting
 *        the bits.
 */
typedef struct
{
  unsigned short limit_lo; // Low 16 bits of seg size
  unsigned short base_lo; // Low 16 bits of seg addr
  unsigned char base_mid; // Middle 8 bits of seg addr

  unsigned char type : 4; // Meaning dependent on several factors
  unsigned char s : 1; // Descriptor type (0 = system, 1 = code/data)
  unsigned char dpl : 2; // Descriptor privilege (Ring level)
  unsigned char p : 1; // Present bit (Set to 1)

  unsigned char limit_hi : 4; // High 16 bits of seg size
  unsigned char avail : 1; // Available for use by sys software
  unsigned char zero : 1; // Don't worry. 64-bit mode only.
  unsigned char one : 1; // Don't worry. 32-bit v. 16-bit
  unsigned g : 1; // If set to 1 aligns on 4KB chunks (page)

  unsigned char base_hi; // High 8 bits of seg addr
} __attribute__((__packed__)) segment_desc_t;

/**
 * Descriptor for Task-state Segment
 * This structure is shown in 
 * the Intel Manual figure 7-2
 */
typedef struct
{
  unsigned short prev; // Previous task link
  unsigned short resv0; // Reserved

  // Static stack segment and offset
  // into stack at specified ring level 
  unsigned esp0;
  unsigned short ss0;
  unsigned short resv1; // Reserved
  unsigned esp1;
  unsigned short ss1;
  unsigned short resv2; // Reserved
  unsigned esp2;
  unsigned short ss2;
  unsigned short resv3; // Reserved

  // 32-bit Registers
  unsigned cr3;
  unsigned eip;
  unsigned eflags;
  unsigned eax;
  unsigned ecx;
  unsigned edx;
  unsigned ebx;
  unsigned esp;
  unsigned ebp;
  unsigned esi;
  unsigned edi;

  // 16-bit regs
  unsigned short es;
  unsigned short resv4; // Reserved

  unsigned short cs;
  unsigned short resv5; // Reserved

  unsigned short ss;
  unsigned short resv6; // Reserved

  unsigned short ds;
  unsigned short resv7; // Reserved

  unsigned short fs;
  unsigned short resv8; // Reserved

  unsigned short gs;
  unsigned short resv9; // Reserved

  unsigned short ldtsel;
  unsigned short resv10; // Reserved

  unsigned short t : 1; // Debug trap flag
  unsigned short resv11 : 15; // Reserved

  unsigned short map_base;
} __attribute__((__packed__)) tss_desc_t;

#endif /** KERNEL_MEM_SEGMENT_DESCRIPTOR_H__ */

