dnl config.m4 for extension posix_addendum

PHP_ARG_ENABLE([posix_addendum],
  [whether to enable posix_addendum support],
  [AS_HELP_STRING([--enable-posix_addendum],
    [Enable posix_addendum support])],
  [no])

if test "$PHP_POSIX_ADDENDUM" != "no"; then
  AC_DEFINE(HAVE_POSIX_ADDENDUM, 1, [ Have posix_addendum support ])

  PHP_NEW_EXTENSION(posix_addendum, posix_addendum.c, $ext_shared)
fi
