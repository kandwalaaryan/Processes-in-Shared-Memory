# IPC Mechanisms: Time Measurement of Processes

This repository contains two C programs demonstrating the use of Inter-Process Communication (IPC) mechanisms to measure the execution time of a command. The programs showcase the use of shared memory and pipes in a POSIX environment.

## Overview

The project includes two versions of a C program to measure the execution time of a process:

1. **time-shm**: This version uses shared memory (shm) to communicate between the parent and child processes. It involves creating a shared memory region to store the start time before the command execution and reading it after the command completes.
2. **time-pipe**: This version uses a UNIX pipe to pass the start time from the child to the parent process. After executing the command, the parent reads the start time from the pipe and calculates the elapsed time.

## Files

- `time-shm.c`: Source code for the shared memory version.
- `time-shm`: Executable for the shared memory version.
- `time-pipe.c`: Source code for the pipe version.
- `time-pipe`: Executable for the pipe version.
- `zombie.c`: An additional file demonstrating how to create a zombie process as part of the learning process.

## Compilation

To compile the programs, use the `gcc` command:

For the shared memory version:

```bash
gcc -o time-shm time-shm.c -lrt
