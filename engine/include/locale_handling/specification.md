# Locale Handling

This file explains the assumptions and rules about the C/C++ Locale. We follow [ut8everywhere](http://utf8everywhere.org/).

## Standard

The global Locale for C and the standard Local for C++ is **en_US.UTF8**. That means that:

- **ALL** text data is **UTF8**.
- **DO NOT TREAT ANY STRING AS ASCII**. This means that ``string[i]``, ``chars[i]`` and its ``length`` are to be treated as byte operations without any meaning in text context.
- UTF8 support in source code and headers is not recomended because it is finicky and sometimes fails (Windows). **Try to use unicode escape codes**.
- Don't count on cout, cin or cerr to handle properly UTF8. This is due to Windows problems, Files works though.
- ``.`` is the decimal separator.
- ``,`` is the digit group delimiter.
- Dates are in the ``mm-dd-yyyy`` format.
- It is better not to do any operation with text in C++ (except memory basics).
- C++ **regex** is **broken** (if necessary, we need a library or it's better to do it on the scripting engine).

## Da Rules

- The Locale Setup must be done before any IO operation.

- **DO NOT** change the C Locale at runtime, it will be a global change that will break any text IO operation happening at the same time (threads).

- **DO NOT** change the global settings of C++ or the ones of cin, cout and cerr.

- **DO NOT** change the numerical locale for anything that must go to other service or be stored. It is okay for UI displaying or reading.

- **ALL** numbers stored must be read and written using **en_US.UTF8**. **No exceptions**.

- **DO NOT USE UTF16** except for syscalls to Windows.
