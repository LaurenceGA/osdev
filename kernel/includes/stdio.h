#ifndef _STDIO_H
#define _STDIO_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

extern int printf(const char *__restrict, ...);
extern int putchar(int);
extern int puts(const char *);

#ifdef _cplusplus
}
#endif

#endif // _STDIO_H
