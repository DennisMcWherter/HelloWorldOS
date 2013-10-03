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
  return 1;
}

void video_write(const char* s)
{
  video_write_color(s, LGRAY);
}

void video_write_color(const char* s, VID_COLOR color)
{
  for( ; *s ; ++s) {
    video_write_char(*s, color);
  }
}

// Code reuse is a good thing:
// The graphics array is a flat array
// Even byte: character to print
// Odd byte: color of character and relevant attribute
void video_write_char(char c, VID_COLOR color)
{
  if(vidptr >= (VID_MEM_ADDR + (VID_WIDTH * VID_HEIGHT))) {
    clear_screen(); // Scroll to top
  }
  *vidptr++ = c;
  *vidptr++ = color;
}

/**
 * Move the cursor down a line
 */
void video_nextline()
{
  // TODO:
  // Eventually want to scroll... Right now
  // We just jump back to the top
  unsigned nextLine = 0, currLine = 0;
  currLine = (((unsigned)vidptr - VID_MEM_ADDR) / VID_WIDTH);
  nextLine = (currLine + 1) % VID_HEIGHT;
  if(nextLine == 0) { // Clear the entire screen and jump to top
    clear_screen(); // Perhaps this is most visually pleasing?
  } else { // Update the video pointer
    vidptr = (unsigned char*)(VID_MEM_ADDR + (nextLine * VID_WIDTH));
  }
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

