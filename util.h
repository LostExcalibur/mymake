#ifndef __UTIL_H
#define __UTIL_H

#include <stddef.h>
#include <stdio.h>

#ifdef _DEBUG
#define DEBUG 1
#else
#define DEBUG 0
#endif

#define debug(fmt, ...)                                                        \
    do {                                                                       \
        if (DEBUG)                                                             \
            printf("%s:%d:%s: " fmt, __FILE__, __LINE__, __func__,             \
                   __VA_ARGS__);                                               \
    } while (0);

#define check_malloc(size) _check_malloc((size), __FILE__, __LINE__)

void *_check_malloc(size_t size, char *file, int line);
void swap(void **a, void **b);

typedef struct {
    char **data;
    size_t capacity;
    size_t length;
} string_da;

string_da *string_da_alloc();
void string_da_free(string_da *da);
void string_da_append(string_da *da, char *value);
void string_da_delete(string_da *da, size_t index);

#endif // __UTIL_H
