#include <stdlib.h>
#include <signal.h>

#include <sighandler.h>

void kill_processes(int sig) {
  int i;
  for(i = 0; i < numver && pids[i] != 0; i++) {
    /* We ignore errors here as there is nothing more we could do */
    kill(pids[i], sig);
  }
}

void init_sighandler() {
  int rc;

  struct sigaction action;
  action.sa_handler = &kill_processes;
  action.sa_mask    = 0;
  action.sa_flags   = 0;

#define sethandler(sig) do {                    \
    rc = sigaction((sig), &action, NULL);       \
    if(rc == -1) {                              \
      perror("sigaction");                      \
      kill_processes(SIGTERM);                  \
      exit(EXIT_FAILURE);                       \
    }                                           \
} while(0)

  sethandler(SIGTERM);
  sethandler(SIGQUIT);
  sethandler(SIGINT);
  sethandler(SIGHUP);
}
