/**
 * k_stdio.h
 *
 * Kernel stdio
 *
 * Author: Dennis J. McWherter, Jr.
 * MIT Lincoln Laboratory
 */

#ifndef K_STDIO_H__
#define K_STDIO_H__

int k_printf(const char*, ...);
int k_putc(char);
void k_itoa(char*, int, int);
void k_panic(const char*);
void k_success(const char*);
int is_big_endian(); // Some augmented functionality

#endif /** K_STDIO_H__ */

