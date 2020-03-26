# PHP POSIX Addendum Extension

A PHP extension providing POSIX functions missing the official [POSIX extension](https://www.php.net/manual/book.posix.php).

## Functions

Currently this extension provides the following POSTIX funtions.

### `int dup2(int oldfd, int newfd)`

Duplicates a file desciptor.

```php
/**
 * Duplicates a stream or file descriptor.
 *
 * @param resource|int $oldStream The old stream to be replaced with the new stream.
 * @param resource|int $newStream The new stream replacing the old stream. 
 *
 * @return bool Returns true on success. Otherwise, returns false.
 */
posix_addendum_dup2(mixed $oldStream, mixed $newStream): bool
```

Example:
```php
posix_addendum_dup2(STDIN, fopen('/dev/null', 'r'));
posix_addendum_dup2(STDOUT, fopen('stdout.log', 'w'));
```

The first line replaces the standard input with `/dev/null`. Hence, all reading from `STDIN` yields 0 bytes.
The second line replaces the standard output with `stdout.log`. Hence, `echo 'Hello, world';` will be written to `stdout.log`.

### `void _exit(int status)`

```php
/**
 * Terminates the calling process "immediately". 
 *
 * All file desciptors belowing to the current process will be closed. However, all registered shutdown
 * functions and all descutors will be bypassed.
 *
 * @param int $status The exit status of the current process.
 */
posix_addendum_immediate_exit(int $status): void
```

Example:
```php

register_shutdown_function(function () {
  echo "awesome exit function\n";
});

class Foo
{
  public function __destruct()
  {
    echo "awesome destructor\n";
  }
}

$foo = new Foo;

posix_addendum_immediate_exit(0);

```
The above program will output nothing and retruns with status 0. However, if you replace `posix_addendum_immediate_exit(0)` with `exit(0)` the program will output:
```
awesome exit function
awesome destructor

```



