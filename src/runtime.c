#include "runtime.h"
#include "kina.h"

int kinart_exitcode_value = 0;

void kinart_init(int argc, char *argv[]) {}

void kinart_main() {
    kinaprog_main();
}

void kinart_cleanup() {}

void kinart_set_exitcode(int code) {
    kinart_exitcode_value = code;
}

int kinart_exitcode() {
    return kinart_exitcode_value;
}
