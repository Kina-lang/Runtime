#include "debug.h"
#include <stdarg.h>
#include <stdio.h>

void kina_debug_print(const char *format, ...) {
  va_list args;

  va_start(args, format);
  
  printf("[KINA RUNTIME] (DEBUG) ");
  vprintf(format, args);
  printf("\n");
  
  va_end(args);
}