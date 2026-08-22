// Main program entry
#include "runtime.h"

int main(int argc, char *argv[]) {
    kinart_init(argc, argv);
    kinart_main();
    kinart_cleanup();

    return kinart_exitcode();
}
