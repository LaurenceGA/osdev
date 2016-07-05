#ifndef _STRING_H
#define _STRING_H

#include <stddef.h>

int memcmp(const void *, const void *, size_t);

void *memcpy(void *__restrict, const void *__restrict, size_t);
void *memmove(void *, const void *, size_t);
void *memset(void *, int, size_t);
void reverse(char *);

size_t strlen(const char *);

#endif // _STRING_H
