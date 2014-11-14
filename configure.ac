AC_PREREQ([2.69])
AC_INIT([mule], [1.0], [tho.feron@gmail.com])
AC_CONFIG_SRCDIR([src/versions.c])
AC_CONFIG_HEADERS([config.h])

AM_INIT_AUTOMAKE([-Wall] [subdir-objects])

AC_PROG_CC

AC_CHECK_HEADERS([stdlib.h stdio.h string.h getopt.h unistd.h dirent.h sys/types.h errno.h pwd.h grp.h signal.h sys/wait.h])

AC_CHECK_FUNCS([strdup getopt_long opendir getpwnam setsuid initgroups sigaction kill chdir execvp wait])

AC_CONFIG_FILES([Makefile src/Makefile])

AC_OUTPUT
