#include "util.h"

#include <stdio.h>
#include <stdlib.h>

void *check_malloc(size_t size) {
  void *ptr = malloc(size);

  if (ptr == NULL) {
    perror("malloc");
    exit(1);
  }

  return ptr;
}

void swap(void** a, void** b) {
	void* tmp = *a;

	*a = *b;

	*b = tmp;
}
