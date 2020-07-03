SB65 Launcher
=

Usage
=

### Interface

Launch from the project root directory:

```
$ ./bin/sb65 [args]
```

#### Arguments

The following arguments are available:

```
sb65 [args]

-h	Display help information
-p	Specify binary path
-r	Specify random seed
-s	Specify display scale
-v	Display version information
```

#### Examples

The following example shows how to launch sb65 with a binary file and random seed:

```
$ ./bin/sb65 -p file.bin -r 12345
```

The following example shows how to launch sb65 with a scaled display:

```
$ ./bin/sb65 -p file.bin -s 3
```
