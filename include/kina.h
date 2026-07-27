#ifndef KINA_H
#define KINA_H

#include <stddef.h>
#include <stdbool.h>

// Kina string struct
struct KinaString {
  char* data;
  int length;
};

// Kina memory management functions (ARC)
void* kina_mem_alloc(size_t size);
void *kina_mem_alloc_global(size_t size);
void kina_mem_retain(void* ptr);
void kina_mem_release(void* ptr);
void kina_mem_free(void* ptr);

// Process argument functions
void kina_process_args_init(int argc, char **argv);
int kina_process_args_get_count();
const char *kina_process_args_get(int index);

#endif // KINA_H
