minishell
=========
A program to emulate a basic shell.

License
-------
This software is 'protected' under the Unlicense and may be utilised freely,
without warranty.

Compiling
---------
Minishell requires your compiler to be able to compile with C11.
Minishell is POSIX-compliant.

Compiling minishell:
```bash
make
```
or, if you want debugging turned on:
```bash
make debug
```

Usage
-----
Running minishell:
```bash
cd bin
./minishell
```

To run any program from within the shell, just write the name of the program.
Like this:
```bash
echo blah
blah
```

To be implemented
-----------------
Several features are currently missing. Here is a list:
  - Running in different environments
  - Truncating paths
  - Doing backflips
