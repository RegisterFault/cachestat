AC_INIT([cachestat],[1.0],[registerfault@gmail.com])
AM_INIT_AUTOMAKE([foreign subdir-objects])
AM_MAINTAINER_MODE
AC_CONFIG_MACRO_DIRS([m4])
AC_PROG_CC
AM_PROG_CC_C_O
AC_CHECK_FUNCS([syscall])
AC_CHECK_HEADERS([linux/mman.h ])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([Makefile src/Makefile])
AC_OUTPUT
