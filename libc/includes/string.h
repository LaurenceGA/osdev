#ifndef _STRING_H
#define _STRING_H

#include <stddef.h>

int memcmp(const void *, const void *, size_t);

void *memcpy(void *__restrict, const void *__restrict, size_t);
void *memmove(void *, const void *, size_t);
void *memset(void *, int, size_t);
void reverse(char *);

size_t strlen(const char *);

char *strcat(char *s, const char *t);
char *strncat(char *s, const char *t, int n);

char *strcpy(char *s, const char *t);
char *strncpy(char *s, const char *t, int n);

int strcmp(const char *s, const char *t);
int strncmp(const char *s, const char *t, int n);

#endif // _STRING_H
