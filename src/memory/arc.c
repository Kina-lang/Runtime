#include "../debug/debug.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct KinaARCMemHeader {
  int ref_count;
  struct KinaARCMemHeader
      *next; // Field for linked list of allocated blocks (for debugging)
} KinaARCMemHeader;

// Macro that takes the data pointer and moves back in the memory to get the
// header
#define KINA_ARC_MEM_GET_HEADER(ptr)                                           \
  ((KinaARCMemHeader *)((char *)(ptr) - sizeof(KinaARCMemHeader)))

// Global linked list of allocated blocks (for debugging)
static KinaARCMemHeader *kina_mem_alloc_blocks_head = NULL;

void *kina_mem_alloc(size_t data_size) {
  // malloc with extra space for the header
  KinaARCMemHeader *header =
      (KinaARCMemHeader *)malloc(sizeof(KinaARCMemHeader) + data_size);
  if (!header)
    return NULL; // Out of memory

  header->ref_count = 1;                     // Initial reference count
  header->next = kina_mem_alloc_blocks_head; // Add to the head of the list
  kina_mem_alloc_blocks_head = header;

  kina_debug_print("ARC MEM: Allocated %zu bytes, ref_count=%d, address=%p",
                   data_size, header->ref_count, (void *)header);

  // Return pointer to the data part, which is after the header
  return (void *)((char *)header + sizeof(KinaARCMemHeader));
}

void kina_mem_free(void *ptr) {
  if (!ptr)
    return; // null pointer check

  KinaARCMemHeader *header = KINA_ARC_MEM_GET_HEADER(ptr);
  free(header); // Free the entire block including the header

  kina_debug_print("ARC MEM: Freed memory at address=%p", (void *)header);

  // Remove from the linked list of allocated blocks
  if (kina_mem_alloc_blocks_head == NULL)
    return; // List is empty

  // If the head is the one to be removed
  if (kina_mem_alloc_blocks_head == header) {
    kina_mem_alloc_blocks_head = header->next; // Remove head
    return;
  }

  KinaARCMemHeader *current = kina_mem_alloc_blocks_head;

  // Traverse the list to find the header and remove it
  while (current->next != NULL && current->next != header) {
    current = current->next;
  }

  // If we found the header, remove it from the list
  if (current->next == header) {
    current->next =
        header->next; // Bypass the header to remove it from the list
  }
}

void kina_mem_retain(void *ptr) {
  if (!ptr)
    return; // null pointer check

  KinaARCMemHeader *header = KINA_ARC_MEM_GET_HEADER(ptr);
  header->ref_count++; // Increment reference count

  kina_debug_print("ARC MEM: Retained, new ref_count=%d, address=%p",
                   header->ref_count, (void *)header);
}

void kina_mem_release(void *ptr) {
  if (!ptr)
    return; // null pointer check

  KinaARCMemHeader *header = KINA_ARC_MEM_GET_HEADER(ptr);
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
    if (current->ref_count > 0) {
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