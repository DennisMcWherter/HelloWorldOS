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

/**
 * Printf implementation
 *
 * @param const char*   NULL-terminated formatted string
 * @param ...           Remaining substitution vars in the format
 * @return  Number of characters printed
 */
int k_printf(const char*, ...);

/**
 * Print a single character
 *
 * @param char    Character to print
 * @return  Character printed
 */
int k_putc(char);

/**
 * Print a string
 *
 * @param const char*   String to print
 * @return  Last character (should be 0)
 */
int k_puts(const char*);

/**
 * Copy an integer to a string
 *
 * @param char*   Destination string
 * @param int     Value to convert
 * @param int     Base to convert in (10 or 16)
 */
void k_itoa(char*, int, int);

/**
 * Print a kernel panic message
 *
 * @param const char*   Message to print
 */
void k_panic(const char*);

/**
 * Print a kernel success message
 *
 * @param const char*   Message to print
 */
void k_success(const char*);

/**
 * Determine if machine is big endian (this is moot
 * though since we are always running x86)
 *
 * @return  1 if big endian 0 otherwise
 */
int is_big_endian(); // Some augmented functionality

#endif /** K_STDIO_H__ */

