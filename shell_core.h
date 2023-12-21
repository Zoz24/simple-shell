#ifndef SHELL_CORE_H
#define SHELL_CORE_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

// Core Shell Functionality
void shell_loop(void);
int shell_execute(char **args);
int shell_launch(char **args);

#endif
