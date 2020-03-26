#ifndef PHP_POSIX_ADDENDUM_H
#define PHP_POSIX_ADDENDUM_H 1

#define PHP_POSIX_ADDENDUM_VERSION "0.0.0"
#define PHP_POSIX_ADDENDUM_EXTNAME "posix_addendum"

#ifdef PHP_WIN32
# define PHP_POSIX_ADDENDUM_API __declspec(dllexport)
#elif defined(__GNUC__) && __GNUC__ >= 4
# define PHP_POSIX_ADDENDUM_API __attribute__ ((visibility("default")))
#else
# define PHP_POSIX_ADDENDUM_API
#endif

static PHP_FUNCTION(posix_addendum_immediate_exit);
static PHP_FUNCTION(posix_addendum_dup2);

extern zend_module_entry posix_addendum_module_entry;
#define phpext_posix_addendum_ptr &posix_addendum_module_entry

#endif
