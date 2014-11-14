#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include <options.h>

#include <start_process.h>

int start_process(options_t *opts, char *version) {
  int rc;

  char *fullpath = NULL;
  asprintf(&fullpath, "%s/%s", opts->dir, version);

  rc = chdir(fullpath);
  if(rc == -1) {
    perror("chdir");
    free(fullpath);
    return -1;
  }

  /* The list of arguments in opts->cmd_argv is not NUL-terminated */
  char **args = (char**)calloc(opts->cmd_argc + 2, sizeof(char *));
  args[0] = opts->cmd;
  int i;
  for(i = 0; i < opts->cmd_argc; i++) {
    args[i+1] = opts->cmd_argv[i];
  }

  rc = execvp(opts->cmd, args);
  if(rc == -1) {
    perror("execvp");
    free(fullpath);
    return -1;
  }
}
