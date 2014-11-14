#ifdef HAVE_CONFIG_H
#  include <config.h>
#else
#  define PACKAGE_STRING "mule (need config.h for version)"
#  define PACKAGE_BUGREPORT "(see website)"
#endif

#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>
#include <string.h>
#include <unistd.h>

#include <options.h>

void show_usage(char *);
void show_version();

options_t *get_options(int argc, char **argv) {
  options_t *opts = (options_t*)calloc(1, sizeof(options_t));

  struct option option_descriptors[] = {
    { "user",       required_argument, NULL, 'u' },
    { "group",      required_argument, NULL, 'g' },
    { "name",       required_argument, NULL, 'n' },
    { "background", no_argument,       NULL, 'b' },
    { "help",       no_argument,       NULL, 'h' },
    { "version",    no_argument,       NULL, 'V' },
    { NULL, 0, NULL, 0 }
  };

  char c;
  while((c = getopt_long(argc, argv, "u:g:n:bhV",
                         option_descriptors, NULL)) != -1) {
    switch(c) {
    case 'u': opts->user  = strdup(optarg); break;
    case 'g': opts->group = strdup(optarg); break;
    case 'n': opts->name  = strdup(optarg); break;
    case 'b': opts->detached = 1; break;
    case 'h': show_usage(argv[0]); exit(EXIT_SUCCESS);
    case 'V': show_version(); exit(EXIT_SUCCESS);
    default:  show_usage(argv[0]); exit(EXIT_FAILURE);
    }
  }

  if(argc - optind < 2) {
    show_usage(argv[0]);
    exit(EXIT_FAILURE);
  }

  opts->dir = argv[optind];
  opts->cmd = argv[optind + 1];

  opts->cmd_argc = argc - optind - 2;
  opts->cmd_argv = argv + optind + 2;

  return opts;
}

void free_options(options_t *opts) {
  if(opts == NULL) { return; }
  free(opts->name);
  free(opts->user);
  free(opts->group);
  free(opts);
}

void show_usage(char *name) {
  fprintf(stderr, "Usage: %s [options] dir cmd [cmd_args]\n\n", name);
  fprintf(stderr, "Options:\n");
  fprintf(stderr, "\t-h --help\t\tShow this help text\n");
  fprintf(stderr, "\t-V --version\t\tShow the version\n");
  fprintf(stderr, "\t-u --user\t\tUsername of the processes\n");
  fprintf(stderr, "\t-g --group\t\tGroup of the processes\n");
  fprintf(stderr, "\nPlease send bug reports to %s.\n", PACKAGE_BUGREPORT);
}

void show_version() {
  printf("%s\n\n", PACKAGE_STRING);
  printf("Copyright (c) 2014 Thomas Feron\n");
  printf("License RBSD: BSD 3-clause <http://www.xfree86.org/3.3.6/COPYRIGHT2.html#5>\n");
  printf("Please send bug reports to %s.\n", PACKAGE_BUGREPORT);
}
