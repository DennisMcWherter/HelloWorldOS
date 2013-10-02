/**
 * drivers/video.c
 *
 * Video driver implementation
 * We have 16KB of video address space
 *
 * Author: Dennis J. McWherter, Jr.
 * MIT Lincoln Laboratory
 */

#include "video.h"

//#define VID_MEM_ADDR 0xc00b8000
#define VID_MEM_ADDR 0xc00b8000
#define VID_HEIGHT 25
#define VID_WIDTH 160


static unsigned char* vidptr = (unsigned char*)VID_MEM_ADDR;
static void clear_screen();

int video_init()
{
  clear_screen(); // Clear the screen
  return 0;
}

void write(const char* s)
{
  write_color(s, LGRAY);
}

void write_color(const char* s, VID_COLOR color)
{
  for( ; *s ; ++s) {
    write_char(*s, color);
  }
}

// Code reuse is a good thing:
// The graphics array is a flat array
// Even byte: character to print
// Odd byte: color of character and relevant attribute
void write_char(char c, VID_COLOR color)
{
  *vidptr++ = c;
  *vidptr++ = color;
}

/** Static methods */
void clear_screen()
{
  // TODO: Consider using more pages
  unsigned i;
  vidptr = (unsigned char*)VID_MEM_ADDR;
  for(i = 0 ; i < (VID_WIDTH * VID_HEIGHT) ; ++i) {
    vidptr[i] = 0;
  }
}

