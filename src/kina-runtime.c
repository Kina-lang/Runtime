#include "./memory/arc.h"
#include "debug/debug.h"
#include "process/args.h"

// Mangled name of the kina main function
extern int kina_program_entry();

int main(int argc, char **argv) {
  kina_debug_print("Starting...");
  kina_process_args_init(argc, argv);

  int exitCode = kina_program_entry();
  kina_debug_print("Program finished with code: %d", exitCode);

  // Cleanup
  if (kina_debug_isEnabled())
    kina_mem_checkLeaks(); // Check for memory leaks before exiting

  return exitCode;
}