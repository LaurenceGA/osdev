#ifndef _STRING_H
#define _STRING_H

#include "stddef.h"

int memcmp(const void *aptr, const void *bptr, size_t n);

void *memcpy(void *__restrict dstptr, const void *__restrict srcptr, size_t n);
void *memmove(void *s1, const void *s2, size_t n);
void *memset(void *s, int c, size_t n);
void *memchr(const void*, int, size_t n);

char *reverse(char *s);

size_t strlen(const char *s);

char *strcat(char *__restrict s, const char *__restrict t);
char *strncat(char *__restrict s, const char *__restrict t, size_t n);

char *strcpy(char *__restrict s, const char *__restrict t);
char *strncpy(char *__restrict s, const char *__restrict t, size_t n);

int strcmp(const char *s1, const char *s2);
int strncmp(const char *s1, const char *s2, size_t n);

char *strchr(const char *s, int c);
char *strrchr(const char *s, int c);

char *strstr(const char *s1, const char *s2);

#endif // _STRING_H
