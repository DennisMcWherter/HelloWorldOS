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

int k_strlen(const char*);
int k_strncmp(const char*, const char*);
int k_strcpy(char*, const char*);
void k_memset(char*, int, int);

#endif /** K_STRING_H__ */

