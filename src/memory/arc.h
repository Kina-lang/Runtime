#ifndef KINA_ARC_H
#define KINA_ARC_H
#include <stddef.h>

void *kina_mem_alloc(size_t data_size);
void kina_mem_retain(void* ptr);
void kina_mem_release(void* ptr);

#endif