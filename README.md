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

Builtin commands: \\
`exit` exits the program \\
`cd` changes directory of host process

To be implemented
-----------------
Several features are currently missing. Here is a list:
  - Run in different environments
  - Truncate paths
  - More builtin commands, like `help`
  - Add args for a more interactive(?) debugging mode
  - Implement redirects (write to files and similar)
  - Implement a way to stop running processes without stopping the shell entirely
  - Find a way to keep track of background processes
  - Possibly find more efficient way to parse strings
  - Add support for arrow keys
  - Have more robust error checking
  - Escape characters somehow
