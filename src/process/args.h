#ifndef KINA_PROCESS_ARGS_H
#define KINA_PROCESS_ARGS_H

void kina_process_args_init(int argc, char **argv);
int kina_process_args_get_count();
const char *kina_process_args_get(int index);

#endif