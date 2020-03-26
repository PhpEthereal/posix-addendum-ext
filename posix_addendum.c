#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_network.h"
#include "php_streams.h"
#include "php_posix_addendum.h"
#include "posix_addendum_arginfo.h"

/* For compatibility with older PHP versions */
#ifndef ZEND_PARSE_PARAMETERS_NONE
#define ZEND_PARSE_PARAMETERS_NONE() \
				ZEND_PARSE_PARAMETERS_START(0, 0) \
				ZEND_PARSE_PARAMETERS_END()
#endif

#if HAVE_SOCKETS || defined(COMPILE_DL_SOCKETS)
#include "ext/sockets/php_sockets.h"
#define POSIX_ADDENDUM_USE_SOCKETS
#endif

/* {{{ php_posix_addendum_zval_to_fd
 * Get numeric file descriptor from PHP stream or Socket resource */
static php_socket_t php_posix_addendum_zval_to_fd(zval *pzfd)
{
	php_socket_t file_desc = -1;
	php_stream *stream;
#ifdef POSIX_ADDENDUM_USE_SOCKETS
	php_socket *php_sock;
#endif
	if (Z_TYPE_P(pzfd) == IS_RESOURCE)
	{
		/* PHP stream or PHP socket resource  */
		if ((stream = (php_stream *) zend_fetch_resource_ex(pzfd, NULL, php_file_le_stream())))
		{
			/* PHP stream */
			if (php_stream_cast(stream,
													PHP_STREAM_AS_FD | PHP_STREAM_CAST_INTERNAL,
													(void*)&file_desc, 1) != SUCCESS || file_desc < 0)
			{
				return -1;
			}
		}
		else
		{
			/* PHP socket resource */
#ifdef POSIX_ADDENDUM_USE_SOCKETS
			if ((php_sock = zend_fetch_resource_ex(pzfd, NULL, php_sockets_le_socket())))
			{
				return php_sock->bsd_socket;
			}

			php_error_docref(NULL, E_WARNING, "either valid PHP stream or valid PHP socket resource expected");

			return -1;
#else
			php_error_docref(NULL, E_WARNING, "valid PHP stream resource expected");

			return -1;
#endif
		}
	}
	else if (Z_TYPE_P(pzfd) == IS_LONG)
	{
		/* Numeric fd */
		file_desc = Z_LVAL_P(pzfd);
		if (file_desc < 0)
		{
			php_error_docref(NULL, E_WARNING, "invalid file descriptor passed");

			return -1;
		}
	}
	else
	{
		/* Invalid fd */
		php_error_docref(NULL, E_WARNING, "invalid file descriptor passed");

		return -1;
	}

	return file_desc;
}
/* }}} */

/* {{{ proto void posix_addendum_immediate_exit(int status);
 * Terminates the calling process "immediately".
 * Does not return. */
PHP_FUNCTION(posix_addendum_immediate_exit)
{
	zend_long status;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "l",  &status) == FAILURE)
	{
		return;
	}

	_exit(status);
}
/* }}} */


/* {{{ proto bool posix_addendum_dup2(mixed fd, mixed fd2);
 * Duplicates a file descriptor.
 * Returns TRUE on success, otherwise FALSE. */
PHP_FUNCTION(posix_addendum_dup2)
{
	zval *zfd, *zfd2;
	int fd, fd2, err;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "zz", &zfd, &zfd2) == FAILURE)
	{
		return;
	}

	fd = php_posix_addendum_zval_to_fd(zfd);
	fd2 = php_posix_addendum_zval_to_fd(zfd2);
	if (fd < 0 || fd2 < 0)
	{
		RETURN_FALSE;
	}

	err = dup2(fd, fd2);
	if (err == -1)
	{
		RETURN_FALSE;
	}

	RETURN_TRUE;
}
/* }}} */

/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(posix_addendum)
{
#if defined(ZTS) && defined(COMPILE_DL_POSIX_ADDENDUM)
	ZEND_TSRMLS_CACHE_UPDATE();
#endif

	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(posix_addendum)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "posix_addendum support", "enabled");
	php_info_print_table_end();
}
/* }}} */

static const zend_function_entry posix_addendum_functions[] =
{
	PHP_FE(posix_addendum_immediate_exit, arginfo_posix_addendum_immediate_exit)
	PHP_FE(posix_addendum_dup2, arginfo_posix_addendum_dup2)
	PHP_FE_END
};

/* {{{ posix_addendum_module_entry
 */
zend_module_entry posix_addendum_module_entry =
{
	STANDARD_MODULE_HEADER,
	PHP_POSIX_ADDENDUM_EXTNAME,
	posix_addendum_functions,
	NULL,
	NULL,
	PHP_RINIT(posix_addendum),
	NULL,
	PHP_MINFO(posix_addendum),
	PHP_POSIX_ADDENDUM_VERSION,
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_POSIX_ADDENDUM
# ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE()
# endif
ZEND_GET_MODULE(posix_addendum)
#endif
