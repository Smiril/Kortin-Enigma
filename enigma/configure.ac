#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.69)
AC_INIT([enigma],[Enigma 0.0.2 T.E.D. - The Enemy Dail - KOENIG MARTIN],[sonar@gmx.com])
AC_CONFIG_AUX_DIR(.)
AM_INIT_AUTOMAKE([-Wall -Werror foreign])
AC_CONFIG_HEADERS([config.h])
ac_clean_files=enigma

# Checks for programs.
AC_LANG([C])
AC_PROG_CC([clang gcc cc])
AC_PROG_CXX([clang++ g++ c++])
AC_PROG_OBJC([clang gcc cc])
AC_PROG_OBJCXX([clang++ g++ c++])

# Checks for header files.
AC_CHECK_HEADERS([stdio.h stdlib.h stdint.h stdarg.h stdbool.h errno.h memory.h unistd.h ctype.h math.h string.h getopt.h sys/types. sys/stat.h sys/time.h iconv.h])

AC_CHECK_FUNCS([strcpy strlcpy isalpha isspace toupper strcmp printf getchar  fopen])

AC_CONFIG_FILES([Makefile src/Makefile ])
AC_OUTPUT
