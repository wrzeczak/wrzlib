# wrzlib

A utility library by and for wrzeczak in use for my projects. Not actively maintained or supported for or by anyone but myself.

### messages.h

Messages is a set of macros for sending error, debug, warning, and todo messages with nice formatting. Error, debug, and warning messages (`eprint`, `dprint`, `wprint`, and their respective `printf` versions) print to the console in a similar format to [Raylib's](https://raylib.com) console logging:

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

R_ARRAY is a simple dynamic array library I wrote mostly for my C rewrite of my [viewer](https://github.com/wrzeczak/viewer) project. It can store integers and strings, doesn't need too much messing around with `malloc()` (hopefully I correctly wrote it in such a way that I don't need to malloc every string ever???), and has a generally pleasing API. If you are in serious need of a dynamic array library, I would not recommend using this; it is not thoroughly tested (other than in `ra_demo.c`) and was written purely to support a personal project. Do not trust me with your data. Documentation on the interface and data structures are provided in `r_array.h`, and I think the code is simple enough to speak for itself.

### wectangle3.h

This is the third iteration of my `Wectangle` library. Raylib `Rectangles` aren't *really* rotateable, and when you do rotate them, they rotate around their top-left corner, not their center. In order to implement 2-d physics are just conveniently rotate rectangles, I created a rectangle which rotates around its center trivially. It's a more-or-less drop in replacement for `Rectangle` (their position `x` and `y` are still relative to the top-left corner, even though they properly rotate around the center). It's not 100% bug-free, but it's pretty much good enough.

Features:
| Feature      | Function |
|--------------|----------|
| Drawing in solid, line, and texture modes. | `DrawWectangle*()`|
| Trivial compatibility with `Rectangle` | `WectangleFromRec` |
| Collision detection between Wec, `Vector2`, and `Rectangle` | `CheckCollision*()`, `GetCollision*()` |

Also, no `malloc()`, like Raylib.
