/**
 * lib/k_string.h
 *
 * Kernel string.h
 *
 * Author: Dennis J. McWherter, Jr.
 * MIT Lincoln Laboratory
 */

#ifndef K_STRING_H__
#define K_STRING_H__

/**
 * String length of a NULL-terminated string
 *
 * @param const char*   String to count
 * @return  String length
 */
int k_strlen(const char*);

/**
 * Compare two strings
 *
 * @param const char*   String one to compare
 * @param const char*   String two to compare
 * @return  0 if strings match else the difference
 *          between the characters
 */
int k_strcmp(const char*, const char*);

/**
 * Copy a string into another
 *
 * @param char*         Destination string
 * @param const char*   Source string
 * @return  Number of characters copied
 */
int k_strcpy(char*, const char*);

/**
 * Set memory of a buffer
 *
 * @param char*   Destination buffer
 * @param int     Value to copy into buffer
 * @param int     Size of the buffer
 */
void k_memset(char*, int, int);

#endif /** K_STRING_H__ */

