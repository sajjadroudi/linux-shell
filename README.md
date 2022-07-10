# Simple Shell

Simple linux shell written in C++. Created as an University project for Operating Systems course. You can study [project_definition.pdf](https://github.com/sajjadroudi/linux-shell/blob/master/project_definition.pdf) for more details.


## Features

- Support basic commands like `exit`, `ls`, `cd` and `cat`.
- Piping implemented (`<cmd1> | <cmd2>`) via `pipe` and `dup2` syscalls. Multiple piping is allowed.
- Aliasing and unaliasing are supported like `alias ll=ls -lah` and `unalias ll`.
- Batch mode is supported using the command `fshell somefile`.
- Program invocation with forking and child processes.
- By pressing Ctrl+C a new prompt will be printed and ready for getting new commands.


## Build and Run

Navigate to the project location.

```bash
  make
```

A file named `main` will be created. Execute it using the following command.

```bash
  ./main
```

