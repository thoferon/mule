#ifndef SIGHANDLER_H
#define SIGHANDLER_H 1

#include <signal.h>

extern pid_t *pids;
extern int numver;

void kill_processes(int);
void init_sighandler();

#endif
