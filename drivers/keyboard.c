/**
 * drivers/keyboard.c
 *
 * Driver implementation
 *
 * Author: Dennis J. McWherter, Jr.
 * MIT Lincoln Laboratory
 */

#include "keyboard.h"
#include "video.h"

#include <kernel/interrupt/interrupt_handler.h>
#include <lib/x86_util.h>

#define KB_PORT 0x60
#define KB_CMD 0x64

static void keyboard_handler(unsigned, registers_t);

int keyboard_init()
{
  // Disable PS/2 ports
  outb(KB_CMD, 0xad); // Disable 1
  outb(KB_CMD, 0xa7); // Disable 2

  inb(KB_PORT); // Flush

  outb(KB_CMD, 0xaa); // Test

  if(inb(KB_PORT) != 0x55)
    return 0;

  // Typically is 1 but since we remapped (to let processor
  // exceptions have lower addresses), it is 32 + n
  register_handler(0x21, keyboard_handler);

  return 1;
}

void keyboard_enable()
{
  outb(KB_CMD, 0xae);
}

void keyboard_disable()
{
  outb(KB_CMD, 0xad);
}

void keyboard_handler(unsigned seg, registers_t reg)
{
  unsigned code = inb(KB_PORT);

  if(code == 0x1e) { // Code for "a" in scan set 1
    video_write("a"); // Fallback on video driver
  }
}

