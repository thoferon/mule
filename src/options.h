#ifndef OPTIONS_H
#define OPTIONS_H 1

typedef struct options {
  int detached;
  char *name;
  char *user;
  char *group;
  char *dir;
  char *cmd;
  int cmd_argc;
  char **cmd_argv;
} options_t;

options_t *get_options(int, char **);
void free_options(options_t *);

#endif
