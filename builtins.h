#ifndef BUILTINS_H
#define BUILTINS_H

// Function declarations for built-in commands
int shell_cd(char **args);
int shell_help(char **args);
int shell_exit(char **args);
int count_builtins(void);

#endif
