/**
 * lib/k_string.c
 *
 * Kernel string.h implementation
 *
 * Author: Dennis J. McWherter, Jr.
 * MIT Lincoln Laboratory
 */

#include "k_string.h"

int k_strlen(const char* str)
{
  int count = 0;
  while(*str++)
    count++;
  return count;
}

int k_strcmp(const char* x, const char* y)
{
  while(*x && *y && *x == *y)
    x++, y++;
  return *x - *y;
}

int k_strcpy(char* dst, const char* src)
{
  char* start = dst;
  while(*src)
    *dst++ = *src++;
  *dst = 0; // NULL terminate
  return start;
}

void k_memset(char* dst, int val, int len)
{
  int i = 0;
  for(i = 0 ; i < len ; ++i) {
    dst[i] = val;
  }
}

