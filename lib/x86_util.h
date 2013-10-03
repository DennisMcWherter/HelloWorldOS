/**
 * lib/x86_util.h
 *
 * Short inline assembly methods to
 * augment capabilities of C
 *
 * Author: Dennis J. McWherter, Jr.
 * MIT Lincoln Laboratory
 */

#ifndef LIB_X86_UTIL_H__
#define LIB_X86_UTIL_H__

static inline void outb(unsigned short port, unsigned char data)
{
  __asm__("\toutb %1, %0\n" : : "Nd"(port), "a"(data));
}

static inline unsigned char inb(unsigned short port)
{
  unsigned char ret = 0;
  __asm__("\tinb %1, %0\n" : "=a"(ret) : "Nd"(port));
  return ret;
}

// I took this method from OSDev wiki
// to work with pic_remap
static inline void io_wait()
{
  __asm__("\toutb %%al, %0\n" : : "Nd"(0));
}

#endif /** LIB_X86_UTIL_H__ */

