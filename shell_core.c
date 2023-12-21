#include "shell_core.h"
#include "builtins.h"
#include "utilities.h"
#include "shared.h"

// Array of built-in command strings
char *builtin_str[] = {
    "cd",
    "help",
    "exit"};

const int builtin_str_size = sizeof(builtin_str) / sizeof(char *);

// Array of built-in command functions
int (*builtin_func[])(char **) = {
    &shell_cd,
    &shell_help,
    &shell_exit};

// Main shell loop
void shell_loop(void)
{
    char *line;
    char **args;
    int status;

    do
    {
        printf("> ");
        line = read_line();
        args = split_lines(line);
        status = shell_execute(args);

        free(line);
        free(args);
    } while (status);
}

// Execute a command
int shell_execute(char **args)
{
    if (args[0] == NULL)
    {
        // Empty command
        return 1;
    }

    for (int i = 0; i < count_builtins(); i++)
    {
        if (strcmp(args[0], builtin_str[i]) == 0)
        {
            return (*builtin_func[i])(args);
        }
    }

    return shell_launch(args);
}

// Launches a new process to execute a command
int shell_launch(char **args)
{
    pid_t pid;
    int status;

    pid = fork();
    if (pid == 0)
    {
        if (execvp(args[0], args) == -1)
        {
            perror("shell");
        }
        exit(EXIT_FAILURE);
    }
    else if (pid < 0)
    {
        perror("shell");
    }
    else
    {
        do
        {
            waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }

    return 1;
}
