# File Load System Specification

This file contains the assumptions that the Load System makes about how the files work

## File Types

There are two types of files:

- Binary Files: Which are read/write as they are.
- Text Files: Which have line endings depending on OS (Windows CRLF and Linux/Mac LF). Runtime only uses LF, this kind of file fixes line endings when read/write.

If a file is not supposed to be opened in any kind of editor or be manually editable, it is okay to use a *Binary File*.

## Unicode

All **Text Files** are considered **UTF8**. When read, they may contain a **BOM** that is removed when passed to the caller. **All** written files are prepended a **BOM** to allow it be opened in an editor on any OS (required by Windows).

Paths are aswell considered **UTF8**.

## Thread Safety

This is **not** a thread safe library. It is suggested to use an IO Thread to avoid locking.

## Cache

This library does **not** cache anything.

## Caller Responsabilities

If the caller wants to use a preallocated buffer to read, it is responsible to close the file.

If the caller wants a write/append file to be indefinetly open, it is responsible to close the file.
