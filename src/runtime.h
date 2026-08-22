#ifndef KINART_RUNTIME_H
#define KINART_RUNTIME_H

void kinart_init(int argc, char *argv[]);
void kinart_main();
void kinart_cleanup();

void kinart_set_exitcode(int code);
int kinart_exitcode();

#endif // KINART_RUNTIME_H
