#include "util.h"

#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void *_check_malloc(size_t size, char *file, int line) {
    debug("Mallocing %zu bytes from %s:%d\n", size, file, line);
    void *ptr = malloc(size);

    if (ptr == NULL) {
        char error_message[50];
        snprintf(error_message, 50, "malloc %s:%d", file, line);
        perror("malloc ");
        exit(1);
    }

    return ptr;
}

void *_check_calloc(size_t nmemb, size_t size, char *file, int line) {
    debug("Callocing %zu * %zu bytes from %s:%d\n", nmemb, size, file, line);
    void *ptr = calloc(nmemb, size);

    if (ptr == NULL) {
        perror("calloc ");
        exit(-1);
    }

    return ptr;
}

void *check_realloc_array(void *ptr, size_t capacity, size_t size) {
    void *tmp = reallocarray(ptr, capacity, size);

    if (tmp == NULL) {
        perror("realloc");
        exit(1);
    }

    return tmp;
}

void swap(void **a, void **b) {
    void *tmp = *a;

    *a = *b;

    *b = tmp;
}

string_da *string_da_alloc() {
    string_da *da = check_malloc(sizeof(string_da));

    da->data = check_calloc(2, sizeof(char *));

    da->length = 0;
    da->capacity = 2;

    return da;
}

void string_da_free(string_da *da) {
    for (size_t i = 0; i < da->length; i++) {
        free(da->data[i]);
    }

    free(da->data);
    free(da);
}

void string_da_append(string_da *da, char *value) {
    assert(da->length <= da->capacity && "Sanity check");
    assert(0 < da->capacity && "Sanity check");
    if (da->length == da->capacity) {
        da->capacity *= 2;
        debug("Reallocing array to %zu elements\n", da->capacity);

        da->data = check_realloc_array(da->data, da->capacity, sizeof(char *));
    }
    debug("%zu elems out of %zu\n", da->length, da->capacity);
    assert(da->length <= da->capacity && "Sanity check");

    da->data[da->length] = value;
    da->length++;
}

void string_da_delete(string_da *da, size_t index) {
    assert(index < da->length && "Out of bounds");

    char **tmp = check_calloc(da->capacity, sizeof(char *));

    char *removed = da->data[index];
    free(removed);

    memmove(tmp, da->data, index * sizeof(char *));
    memmove(tmp + index, da->data + index + 1,
            (da->length - index - 1) * sizeof(char *));

    free(da->data);
    da->data = tmp;

    da->length -= 1;
}
