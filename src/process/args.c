#include "args.h"

#include <stdlib.h>

#include "../debug/debug.h"

static int arg_count = 0;
static char **arg_values = NULL;

void kina_process_args_init(int argc, char **argv) {
  kina_debug_print("PROC ARGS: Got %d args", argc);

  arg_count = argc;
  arg_values = argv;
}

int kina_process_args_get_count() { return arg_count; }

const char *kina_process_args_get(int index) {
  if (index < 0 || index >= arg_count) {
    return NULL; // Out of bounds
  }

  return arg_values[index];
}