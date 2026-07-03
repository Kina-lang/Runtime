#include <stdio.h>

// Mangled name of the kina main function
extern int kina_program_entry();

int main(int argc, char **argv) {
  printf("[KINA RUNTIME] Starting...\n");

  int result = kina_program_entry();

  printf("[KINA RUNTIME] Program finished with code: %d\n", result);

  return result;
}