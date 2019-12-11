# Code-style

This cocument specifies the naming of code-related things and overall design
decisions. For git naming conventions and development practices, refer to
git-style. One shouldn't worry with code formatting as it is automatically
handled by the project's clang-format. To format the codebase, run:

```$ make format```

## Naming things

Please respect the following:

- Variables and methods in `lower_case`.
- Constants in `ALL_CAPS`.
- Classes, Enum, Structs and typedefs in `UpperCase`.
- Private class attributes in `_prefixed_lower_case`.
- Interface names should reflect their provided functionality, i.g: `Printer`.

## Commentaries

All the following are 80 columns wide.

To-Dos should follow:

```
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//
// @TODO: What should be done
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//
```

Big sections of code (found in .cc files), should be identified with the
following pattern (prefixed by 8 underscores):

```
/*________SECTION PURPOSE____________________________________________________*/
```

Black-magic code should always come with an explanation, such as in:

```
//=============AUTOMAKE======================================================//
// Since it's possible to include files within GNU make, this build system
// relies on GCC preprocessor's flags (CPPFLAGS) -MD -MP to generate make
// compliant target rules for each .c and .cc file. It does so by including
// the generated .d files. GNU make is able to turn .cc and .c files into
// dependency files, .d implicitly by using the aforementioned C-preprocessor.
//
// For more information on this technique please refer to the links:
//  - http://make.mad-scientist.net/papers/advanced-auto-dependency-generation/
//  - http://www.microhowto.info/howto/automatically_generate_makefile_
// 	dependencies.html
//===========================================================================//

-include $(CXX_SRC:.cc=.d)
-include $(C_SRC:.c=.d)
```

On certain situations, it may be necessary to write small comments to explain
opaque code, such as in:

```
// mask interrupts
out8(0x21, 0xff);
out8(0xA1, 0xff);
```

In such cases, prefer to write single-lined, lower case comments.

## Design patterns

Some of the following patterns are under debate. In such cases, follow what's
been done in other parts of the codebase.

### Hardware mediators

When designing a hardware mediator, say `Mediator`, one should strive to provide
the biggest possible architecture-free interface under the name `Mediator_Common`.
The inevitable hardware dependencies should be placed in `Mediator`. If the
hardware mediator needs prior initialization before it's objects can be
instantiated, then it should provide a `init` function.

`Mediator_Common` should be placed under `include/machine/mediator.h` and it's
implementation should be under `src/machine/mediator.cc`. The same applies to
`Mediator`, which should be placed under `include/machine/MACH_NAME/mediator.h`.

### namespace _setup (under debate)

In order to avoid a grotesque `_pre_lib_init`, the namespace `_setup` is provided
to hide internal objects used for setting up other global objects that constitute
the `std` namespace.

### Header files

All header files should be placed under `include` and should be guarded by
include guards. Header files should not contain any implementation code (with
the except of one-liners). All symbols which other files would need from a given
header file should be forward defined (extern).

### Class organization (under debate)

Classes should be structured in the following order:

1. public interface: typedefs, friend classes, nested classes, constants, static
member methods, member methods, static member variables and member variables.
2. private interface: in the same order, variables prefixed with underscore.

Implementation should be driven in the same manner. The only difference being
that large code sections should come with the aforementioned section coment.