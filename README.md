# wrzlib

A utility library by and for wrzeczak in use for my projects. Not actively maintained or supported for or by anyone but myself.

### messages.h

Messages is a set of macros for sending error, debug, warning, and todo messages with nice formatting. Error, debug, and warning messages (`eprint`, `dprint`, `wprintf`, and their respective `printf` versions) print to the console in a similar format to Raylib's console logging:

```
WRZ: DEBUG: This is a debug message. These are toggled at compile-time with the -DDEBUG flag to gcc.
WRZ: WARN: These warning messages are used for non-critical errors, trying to do something potentially dangerous or nonsensical but not program-ending.

WRZ: ERROR: These print with newlines surrounding and are used for critical, program-ending errors.
```

Additionally, there is the `todo` macro, which lets the user know when they call a function:

```c
void gabagool(void) {
  todo("gabagool");
}
```
```
> ./gabagool

WRZ: TODO: function gabagool() is not yet implemented!

Exited with error code -2.
```

that said function is not yet implemented, exiting the program. I use this in files when I intend or need a feature but haven't gotten around to implementing it.

### r_array.h

R_ARRAY is a simple dynamic array library I wrote mostly for my C rewrite of my [viewer](https://github.com/wrzeczak/viewer) project. It can store integers and strings, doesn't need too much messing around with `malloc()` (hopefully I correctly wrote it in such a way that I don't need to malloc every string ever???), and has a generally pleasing API. If you are in serious need of a dynamic array library, I would not recommend using this; it is not throughly tested (other than in `ra_demo.c`) and was written purely to support a personal project. Do not trust me with your data. Documentation on the interface and data structures are provided in `r_array.h`, and I think the code is simple enough to speak for itself.
