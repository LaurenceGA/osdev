#ifndef _STDLIB_H
#define _STDLIB_H

#include "stddef.h"

typedef struct div_t {
	int quot;
	int rem;
} div_t;

typedef struct ldiv_t {
	long quot;
	long rem;
} ldiv_t;

char *itoa(int, char *, int);
int atoi(const char *, int);
long atol(const char *, int);
long long atoll(const char *, int);

div_t div(int numer, int denom);
ldiv_t ldiv(long number, long denom);

int abs(int j);
long labs(long j);

void free(void *ptr);
void *malloc(size_t size);
void *realloc(void *ptr, size_t size);
void *calloc(size_t nmemb, size_t size);

#endif // _STDLIB_H
