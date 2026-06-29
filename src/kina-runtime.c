#include <stdio.h>

// Mangled name of the kina main function
extern int k_fn_Z4main();

int main(int argc, char **argv) {
  printf("[KINA RUNTIME] Starting...\n");

  int result = k_fn_Z4main();

  printf("[KINA RUNTIME] Program finished with code: %d\n", result);

  return result;
}