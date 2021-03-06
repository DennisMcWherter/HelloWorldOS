/**
 * drivers/video.h
 *
 * Video driver header. We will only
 * be working in text mode (graphics mode
 * is a more difficult concept)
 *
 * Author: Dennis J. McWherter, Jr.
 * MIT Lincoln Laboratory
 */

#ifndef DRIVERS_VIDEO_H__
#define DRIVERS_VIDEO_H__

/**
 * Video driver colors
 * Reference:
 * http://en.wikipedia.org/wiki/Color_Graphics_Adapter
 */
typedef enum
{
  BLACK    = 0x00,
  BLUE     = 0x01,
  GREEN    = 0x02,
  CYAN     = 0x03,
  RED      = 0x04,
  MAGENTA  = 0x05,
  BROWN    = 0x06,
  LGRAY    = 0x07,
  DGRAY    = 0x08,
  LBLUE    = 0x09,
  LGREEN   = 0x0a,
  LCYAN    = 0x0b,
  LRED     = 0x0c,
  LMAGENTA = 0x0d,
  LBROWN   = 0x0e,
  WHITE    = 0x0f
} VID_COLOR;

/**
 * Initialize video driver
 *
 * @return  1 on success 0 otherwise
 */
int video_init();

/**
 * Method to write out to screen
 *
 * @param const char*   NULL-terminated string to write
 */
void video_write(const char*);

/**
 * Write color output to screen
 *
 * @param const char*   NULL-terminated string to write
 * @param VID_COLOR     Color code of the string (VGA)
 */
void video_write_color(const char*, VID_COLOR);

/**
 * Write a single char t othe output
 *
 * @param char        Character to write
 * @param VID_COLOR   Color code of char (VGA)
 */
void video_write_char(char, VID_COLOR);

/**
 * Force terminal to a new line
 */
void video_nextline();

#endif /** DRIVERS_VIDEO_H__ */

