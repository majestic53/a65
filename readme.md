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
TBD
```

Changelog
=========

Version 0.1.1839
----------------
*Updated: 9/26/2018*

* Implemented section class

*Updated: 9/25/2018*

* Added assembler class
* Added section class
* Added negative scalar support

Version 0.1.1838
----------------
*Updated: 9/19-20/2018*

* Implemented parser enumeration routines
* Parser/lexer bug fixes

*Updated: 9/18/2018*

* Converted binary tree class to d-ary tree class

Version 0.1.1837
----------------
*Updated: 9/13/2018*

* Implemented parser node/tree classes

*Updated: 9/11/2018*

* Completed lexer class
* Added parser node/tree classes

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
