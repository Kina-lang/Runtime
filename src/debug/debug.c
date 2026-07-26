#include "debug.h"
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>

bool kina_debug_enabled = false;

void kina_debug_toggle(bool enabled) { kina_debug_enabled = enabled; }

bool kina_debug_isEnabled() { return kina_debug_enabled; }

void kina_debug_print(const char *format, ...) {
  if (!kina_debug_enabled)
    return;

  va_list args;

  va_start(args, format);

  printf("[KINA RUNTIME] (DEBUG) ");
  vprintf(format, args);
  printf("\n");

  va_end(args);
}