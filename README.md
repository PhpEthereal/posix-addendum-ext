# PHP POSIX Addendum Extension

A PHP extension providing POSIX functions missing the official [POSIX extension](https://www.php.net/manual/book.posix.php).

## Functions

Currently this extension provides the following POSTIX funtions.

### `int dup2(int oldfd, int newfd)`

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





