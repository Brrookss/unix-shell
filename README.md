# unix-shell

## Description

This is a program that includes functionalities commonly provided by well-known
shells, such as Bash and Z shell.

Some functionalities include:
1. Variable expansion:
    - ```$$``` displays the process ID of the shell
2. I/O redirection:
    - ```[< input_file]``` and ```[> output_file]```
3. Support for commands:
    - Three built-in, specifically, ```cd```, ```exit```, and ```status```
    - Common Unix commands (e.g.,  ```date```, ```echo```, and ```sleep```)
    which are handled through system calls
4. Ability to execute commands in the background:
    - Any non-built-in commands ending with ```&``` retain command-line access
5. Signal handling:
    - SIGINT, invoked with ```CTRL-C```, terminates any foreground processes,
    excluding the shell
    - SIGTSTP, invoked with ```CTRL-Z```, cycles between "normal" and
    "foreground-only" mode
6. Support for comments:
    - Lines starting with ```#``` are ignored

## Getting Started

### Dependencies

- GNU Compiler Collection (GCC)

### Installing

1. Compile and create executable using Makefile via ```make``` 

### Executing program

1. Run program

```./shell```

2. Execute commands using the following command-line syntax:

```command [arg1 arg2 ... argN] [< input_file] [> output_file] [&]```

3. Exit program

```exit```

## Notes

- ```cd``` supports both absolute and relative paths. However, without any
arguments, the current directory changes to the directory specified in the HOME
environment variable.

- ```status``` displays the exit status of the most recent non-built-in
foreground process.

- Commands have a maximum length of 2048 characters and 512 arguments.

## Authors

[Brooks Burns](https://github.com/Brrookss)