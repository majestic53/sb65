SB65
=

[![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0) [![Build Status](https://travis-ci.com/majestic53/sb65.svg?branch=master)](https://travis-ci.com/majestic53/sb65)

About
=

A 65C02 emulator sandbox environment, written in C

Table of Contents
=

1. [Building](https://github.com/majestic53/sb65#building) -- How to build the project
2. [Usage](https://github.com/majestic53/sb65#usage) -- How to use the project
3. [License](https://github.com/majestic53/sb65#license) -- Project license

Building
=

__NOTE__: Tested under Linux with the GCC/Clang C compilers. Modification to the makefiles might be required to build with a different OS/compiler.

### Preparing to build

The following libraries are required to build this project:

```
libsdl2
```

### Building

Run the following commands from the projects root directory:

```
$ export CC=<COMPILER>
$ make <BUILD>
```

Where COMPILER corrisponds to either ```clang```, ```gcc``` or the compiler of your choice.

Where BUILD corrisponds to either ```rel``` (release) or ```dbg``` (debug). If no BUILD argument is specified, a release build will be created.

Usage
=

### Interface

The project is implemented in C and exposes a simple API, described in ```include/sb65.h``` and ```lib/libsb65.a```:

For an example, see the launcher tool under ```tool/```

#### Runtime Routines

|Name    |Description    |Signature                              |
|:-------|:--------------|:--------------------------------------|
|sb65_run|Run environment|```int sb65_run(const sb65_conf_t *)```|

#### Misc. Routines

|Name        |Description          |Signature                            |
|:-----------|:--------------------|:------------------------------------|
|sb65_error  |Retrieve error string|```const char *sb65_error(void)```   |
|sb65_version|Retrieve version     |```void sb65_version(sb65_ver_t *)```|

License
=

SB65 is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

SB65 is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
