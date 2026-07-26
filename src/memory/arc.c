#include "../debug/debug.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct KinaARCMemHeader {
  int ref_count;
  bool is_global; // Flag to indicate if the memory is kept for the entire
                  // program lifetime (is not freed/leaked)
  struct KinaARCMemHeader
      *next; // Field for linked list of allocated blocks (for debugging)
} KinaARCMemHeader;

// Macro that takes the data pointer and moves back in the memory to get the
// header
#define KINA_ARC_MEM_GET_HEADER(ptr)                                           \
  ((KinaARCMemHeader *)((char *)(ptr) - sizeof(KinaARCMemHeader)))

// Global linked list of allocated blocks (for debugging)
static KinaARCMemHeader *kina_mem_alloc_blocks_head = NULL;

void *kina_mem_alloc_impl(size_t data_size, bool is_global) {
  // malloc with extra space for the header
  KinaARCMemHeader *header =
      (KinaARCMemHeader *)malloc(sizeof(KinaARCMemHeader) + data_size);
  if (!header)
    return NULL; // Out of memory

  header->ref_count = 1;                     // Initial reference count
  header->is_global = is_global;             // Set the global flag
  header->next = kina_mem_alloc_blocks_head; // Add to the head of the list
  kina_mem_alloc_blocks_head = header;

  kina_debug_print(
      "ARC MEM: Allocated %zu bytes, ref_count=%d, address=%p, is_global=%d",
      data_size, header->ref_count, (void *)header, header->is_global);

  // Return pointer to the data part, which is after the header
  return (void *)((char *)header + sizeof(KinaARCMemHeader));
}

void *kina_mem_alloc(size_t size) { return kina_mem_alloc_impl(size, false); }

void *kina_mem_alloc_global(size_t size) {
  return kina_mem_alloc_impl(size, true);
}

void kina_mem_free(void *ptr) {
  if (!ptr)
    return; // null pointer check

  KinaARCMemHeader *header = KINA_ARC_MEM_GET_HEADER(ptr);

  // Remove from the linked list of allocated blocks first, while header is
  // valid
  if (kina_mem_alloc_blocks_head != NULL) {
    if (kina_mem_alloc_blocks_head == header) {
      kina_mem_alloc_blocks_head = header->next;
    } else {
      KinaARCMemHeader *current = kina_mem_alloc_blocks_head;

      while (current->next != NULL && current->next != header) {
        current = current->next;
      }

      if (current->next == header) {
        current->next = header->next;
      }
    }
  }

  if (header->is_global) {
    kina_debug_print("ARC MEM: ATTEMPTED TO FREE GLOBAL MEMORY! Object at "
                     "address=%p, skipping free.",
                     (void *)header);
    return; // Do not free global memory
  }

  kina_debug_print("ARC MEM: Freed memory at address=%p, is_global=%d",
                   (void *)header, header->is_global);
  free(header); // Free the entire block including the header
}

void kina_mem_retain(void *ptr) {
  if (!ptr)
    return; // null pointer check

  KinaARCMemHeader *header = KINA_ARC_MEM_GET_HEADER(ptr);
  if (header->ref_count < 0)
    return; // Static memory object, do nothing

  header->ref_count++; // Increment reference count

  kina_debug_print("ARC MEM: Retained, new ref_count=%d, address=%p",
                   header->ref_count, (void *)header);
}

void kina_mem_release(void *ptr) {
  if (!ptr)
    return; // null pointer check

  KinaARCMemHeader *header = KINA_ARC_MEM_GET_HEADER(ptr);
  if (header->ref_count < 0)
    return; // Static memory object, do nothing

  header->ref_count--; // Decrement reference count

  kina_debug_print("ARC MEM: Released, new ref_count=%d, address=%p",
                   header->ref_count, (void *)header);

  // If reference count drops to zero, free the memory
  if (header->ref_count <= 0) {
    kina_mem_free(ptr);
  }
}

void kina_mem_checkLeaks() {
  KinaARCMemHeader *current = kina_mem_alloc_blocks_head;
  int leaked = 0;

  while (current != NULL) {
    if (current->ref_count > 0 && !current->is_global) {
      kina_debug_print(
          "ARC MEM: MEMORY LEAK DETECTED! Object at %p has ref_count %d",
          (void *)((char *)current + sizeof(KinaARCMemHeader)),
          current->ref_count);
      leaked++;
    }

    current = current->next;
  }

  kina_debug_print("ARC MEM: %d memory leaks.", leaked);
}