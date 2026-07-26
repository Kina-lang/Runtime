#ifndef KINA_DEBUG_H
#define KINA_DEBUG_H

#include <stdbool.h>

bool kina_debug_isEnabled();
void kina_debug_print(const char *message, ...);

#endif