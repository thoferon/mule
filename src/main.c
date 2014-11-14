#include <stdio.h>
#include <stdlib.h>
#include <pwd.h>
#include <grp.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <sys/wait.h>

#include <options.h>
#include <versions.h>
#include <sighandler.h>
#include <start_process.h>

pid_t *pids;
int numver;

int main(int argc, char **argv) {
  int rc;
  pid_t pid;
  int i;

  options_t *opts = get_options(argc, argv);

  char *fullname = NULL;
  asprintf(&fullname, "%s:", opts->name ? opts->name : argv[0]);
  argv[0] = fullname;

  if(opts->detached) {
    pid = fork();
    if(pid == 0) {
      rc = setsid();
      if(rc == -1) {
        perror("setsid");
        return EXIT_FAILURE;
      }
    } else if(pid == -1) {
      perror("fork");
      return EXIT_FAILURE;
    } else {
      return EXIT_SUCCESS;
    }
  }

  if(opts->group) {
    struct group  *grp = getgrnam(opts->group);
    if(grp == NULL) {
      fprintf(stderr, "getgrnam(%s): Couldn't find group\n", opts->group);
      return EXIT_FAILURE;
    }

    rc = setgid(grp->gr_gid);
    if(rc != 0) {
      fprintf(stderr, "setgid(%d): Couldn't set the group ID\n", grp->gr_gid);
      return EXIT_FAILURE;
    }

    rc = initgroups(opts->group, grp->gr_gid);
    if(rc == -1) {
      fprintf(stderr, "initgroups(%s, %d): You must run the program as root\n",
              opts->group, grp->gr_gid);
      return EXIT_FAILURE;
    }
  }

  if(opts->user) {
    struct passwd *pwd = getpwnam(opts->user);
    if(pwd == NULL) {
      fprintf(stderr, "getpwnam(%s): Couldn't find user\n", opts->user);
      return EXIT_FAILURE;
    }

    rc = setuid(pwd->pw_uid);
    if(rc != 0) {
      fprintf(stderr, "setuid(%d): Couldn't set the user ID\n", pwd->pw_uid);
      return EXIT_FAILURE;
    }
  }

  char **versions;
  numver = get_versions(opts->dir, &versions);

  pids = (pid_t*)calloc(numver, sizeof(pid_t));

  for(i = 0; i < numver; i++) {
    pid = fork();

    switch(pid) {
    case -1:
      perror("fork");
      kill_processes(SIGTERM);
      return EXIT_FAILURE;
    case 0:
      rc = start_process(opts, versions[i]);
      if(rc == -1) {
        /* This is in a child process that might have a "smaller" version of the
         * list of PIDs. Some processes might then not be killed here.
         * This is probably good enough corresponding the scope of this program.
         */
        kill_processes(SIGTERM);
        return EXIT_FAILURE;
      }
    default:
      pids[i] = pid;
    }
  }

  init_sighandler();

  asprintf(&argv[1], "%i/%i remaining processes", numver, numver);
  argv[2] = NULL;

  int status;
  while((pid = wait(&status)) != -1 || errno == EINTR) {
    if(WIFEXITED(status) || WIFSIGNALED(status)) {
      int nrem = 0;
      for(i = 0; i < numver; i++) {
        if(pids[i] == pid) { pids[i] = 0; }
        if(pids[i] != 0)   { nrem++; }
      }
      asprintf(&argv[1], "%i/%i remaining processes", nrem, numver);
      if(nrem == 0) {
        break;
      }
    }
  }

  free(fullname);
  free_options(opts);
  return EXIT_SUCCESS;
}
