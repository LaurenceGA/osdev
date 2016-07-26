#include "stdlib.h"
#include "memory.h"

void free(void *ptr) {

}

void *malloc(size_t size) {
	return NULL;
}

void *realloc(void *ptr, size_t size) {
	if (ptr == NULL)
		return malloc(size);
	return NULL;
}

void *calloc(size_t nmemb, size_t size) {
	return NULL;
}
