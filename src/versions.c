#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <sys/types.h>
#include <string.h>

#include <versions.h>

int get_versions(char *dirpath, char ***versions_ptr) {
  int rc;

  DIR *dir = opendir(dirpath);
  if(dir == NULL) {
    perror("opendir");
    exit(EXIT_FAILURE);
  }

  size_t arrsize = 8;
  char **versions = (char**)malloc(arrsize * sizeof(char*));

  int numver = 0;

  struct dirent *dirent;
  while((dirent = readdir(dir)) != NULL) {
    if(dirent->d_name[0] == '.' || dirent->d_type != DT_DIR) {
      continue;
    }

    numver++;
    if(numver > arrsize) {
      versions = (char**)realloc(versions, arrsize * sizeof(char*));
    }

    char *name = (char*)malloc(dirent->d_namlen + 1);
    strncpy(name, dirent->d_name, dirent->d_namlen + 1);
    versions[numver - 1] = name;
  }

  rc = closedir(dir);
  if(rc == -1) {
    perror("closedir");
    exit(EXIT_FAILURE);
  }

  *versions_ptr = versions;

  return numver;
}
