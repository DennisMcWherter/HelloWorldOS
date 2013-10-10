/**
 * k_stdio.c
 *
 * Kernel stdio implementation
 *
 * Author: Dennis J. McWherter, Jr.
 * MIT Lincoln Laboratory
 */

#include "k_stdio.h"
#include "k_string.h"
#include <drivers/video.h> // Use driver directly

// Define builtin varargs
#define va_list __builtin_va_list
#define va_arg(x, y) (__builtin_va_arg(x, y))
#define va_start(x, y) (__builtin_va_start(x, y))
#define va_end(x) (__builtin_va_end(x))

// Static
static void reverse(char*);

char hexLUT[] = "0123456789abcdef";

int k_printf(const char* fmt, ...)
{
  char buf[32];
  int i = 0;
  int count = 0;
  char* str = 0;
  va_list args;

  va_start(args, fmt);
  // Determine how many arguments we have
  // This is an over-simplified printf
  for(i = 0 ; fmt[i] ; ++i) {
    switch(fmt[i]) {
      case '%':
        fmt++; // Advance pointer
        if(fmt[i] == 0)
          break;
        switch(fmt[i]) { // Check what arg we have
          case 'i':
          case 'd':
            k_memset(buf, 0, sizeof(buf));
            k_itoa(buf, va_arg(args, int), 10);
            video_write(buf);
            count += k_strlen(buf);
            break;
          case 'x':
            k_memset(buf, 0, sizeof(buf));
            k_itoa(buf, va_arg(args, unsigned), 16);
            video_write(buf);
            count += k_strlen(buf);
            break;
          case 's':
            str = va_arg(args, char*);
            k_puts(str);
            count += k_strlen(buf);
            break;
          case '%':
          default:
            // Ignore-- not supported
            k_putc(fmt[i]);
            count++;
            break;
        }
        break;
      case '\r':
      case '\n':
        video_nextline();
        count++;
        break;
      default: // Write the char
        k_putc(fmt[i]);
        count++;
        break;
    }
  }
  va_end(args);

  return count;
}

void k_success(const char* msg)
{
  video_write("[");
  video_write_color("SUCCESS", GREEN);
  video_write("] ");
  k_printf(msg);
}

void k_panic(const char* msg)
{
  video_write("[");
  video_write_color("!!!PANIC", RED);
  video_write("] ");
//  video_write(msg);
  k_printf(msg);
}

int k_putc(char c)
{
  video_write_char(c, LGRAY);
  return (int)c;
}

int k_puts(const char* str)
{
  while(*str) {
    k_putc(*str++);
  }
  return *str;
}

void k_itoa(char* buf, int val, int radix)
{
  char* start = buf;
  unsigned v2;

  // Only supporting hex and decimal for now
  radix = (radix == 16) ? radix : 10;

  // Ignore negative values for hex
  if(val < 0 && radix != 16) {
    start++;
    *buf++ = '-';
    val *= -1;
  }

  v2 = (unsigned)val; // Do unsigned math.

  do {
    *buf++ = hexLUT[(v2 % radix)];
    v2 /= radix;
  } while(v2);

  // Hack: Just don't want to spend too much
  // time reimplementing c stdlib
  if(!is_big_endian())
    reverse(start);
}

int is_big_endian()
{
  union {
    int x;
    char c[sizeof(int)];
  } val;
  val.x = 0xdeadbeef;
  return val.c[0] == (char)0xde;
}

/** Static methods */
void reverse(char* str)
{
  int len = k_strlen(str);
  int i = 0;
  char tmp;
  for(i = 0 ; i < len/2 ; ++i) {
    tmp    = str[i];
    str[i] = str[len - i - 1];
    str[len - i - 1] = tmp;
  }
}


