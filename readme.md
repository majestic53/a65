A65
===

A small 6502 assembler, written in C++

Table of Contents
=================

1. [Build](https://github.com/majestic53/a65#build) -- How to build the project
2. [Usage](https://github.com/majestic53/a65#usage) -- How to use the project
3. [Changelog](https://github.com/majestic53/a65#changelog) -- Project changelist information
4. [License](https://github.com/majestic53/a65#license) -- Project license information

Build
=====

__NOTE__: Tested with the Clang C++ compiler. Modification to the makefiles might be required to build with a different compiler.

Run the following command from the projects root directory:

```
make release
```

To build with tracing enabled, run the following command from the project's root directory:

```
$ make TRACE=<LEVEL>
```

Where LEVEL corrisponds to the tracing verbosity (0=Error, 1=Warnings, 2=Information, 3=Verbose)

Usage
=====

Launch from the project root directory:

```
./bin/a65 [ARGS]
```

### Interface

```
a65 [-a name][-c name][-e][-h][-i][-n][-o output][-s][-b][-v] input...

-a|--archive     Output archive file
-c|--compile     Output binary file
-e|--export      Output export file
-h|--help        Display help information
-i|--ihex        Output ihex file
-n|--no-binary   Exclude binary file output
-o|--output      Specify output directory
-s|--source      Enable source output
-b|--verbose     Enable verbose output
-v|--version     Display version information
```

### Example

1. To build objects files, call a65 with a series of source files as input:

```
a65 -o <OUTPUT> <INPUT>

For example: a65 -o ./bin ./doc/example.asm
```

2. To build archives files, call a65 with a series of source and/or object files as input and a name:

```
a65 -a <NAME> -o <OUTPUT> <INPUT>

For example: a65 -a example -o ./bin ./doc/example.asm
```

3. To build binary files, call a65 with a series of source, object and/or archive files as input and a name:

```
a65 -c <NAME> -o <OUTPUT> <INPUT>

For example: a65 -c example -o ./bin ./doc/example.asm
```

Changelog
=========

Version 0.1.1843
----------------
*Updated: 10/23/2018*

* Added additional interface flags
* Implemented binary output (cont.)

*Updated: 10/22/2018*

* Implemented binary output
* Numerous bug fixes

Version 0.1.1842
----------------
*Updated: 10/19/2018*

* Implemented command evaluation

*Updated: 10/16/2018*

* Implemented directive evaluation
* Simplified archive file format

*Changelog truncated (see file history for full log)*

License
=======

Copyright(C) 2018 David Jolly <majestic53@gmail.com>

A65 is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

A65 is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
