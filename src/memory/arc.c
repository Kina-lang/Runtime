#include <stdlib.h>
#include <stdio.h>
#include "../debug/debug.h"

typedef struct {
  int ref_count;
} KinaARCMemHeader;

// Macro that takes the data pointer and moves back in the memory to get the header
#define KINA_ARC_MEM_GET_HEADER(ptr) ((KinaARCMemHeader*)((char*)(ptr) - sizeof(KinaARCMemHeader)))

void *kina_mem_alloc(size_t data_size) {
  // malloc with extra space for the header
  KinaARCMemHeader* header = (KinaARCMemHeader*)malloc(sizeof(KinaARCMemHeader) + data_size);
  if (!header) return NULL; // Out of memory

  header->ref_count = 1; // Initial reference count

  kina_debug_print("ARC MEM: Allocated %zu bytes, ref_count=%d, address=%p", data_size, header->ref_count, (void*)header);

  // Return pointer to the data part, which is after the header
  return (void*)((char*)header + sizeof(KinaARCMemHeader));
}

void kina_mem_retain(void* ptr) {
  if (!ptr) return; // null pointer check

  KinaARCMemHeader* header = KINA_ARC_MEM_GET_HEADER(ptr);
  header->ref_count++; // Increment reference count

  kina_debug_print("ARC MEM: Retained, new ref_count=%d, address=%p", header->ref_count, (void*)header);
}

void kina_mem_release(void* ptr) {
  if (!ptr) return; // null pointer check

  KinaARCMemHeader* header = KINA_ARC_MEM_GET_HEADER(ptr);
  header->ref_count--; // Decrement reference count

  kina_debug_print("ARC MEM: Released, new ref_count=%d, address=%p", header->ref_count, (void*)header);

  // If reference count drops to zero, free the memory
  if (header->ref_count <= 0) {
    free(header); // Free the entire block including the header

    kina_debug_print("ARC MEM: Freed memory at address=%p", (void*)header);
  }
}