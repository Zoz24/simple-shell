#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

#define LINE_BUFSIZE 1024
#define TOKEN_BUFSIZE 64
#define TOKEN_DELIM " \t\r\n\a"

// Function declarations
int shell_cd(char **args);
int shell_help(char **args);
int shell_exit(char **args);

// Array of built-in command strings
char *builtin_str[] = {
    "cd",
    "help",
    "exit"};

// Array of built-in command functions
int (*builtin_func[])(char **) = {
    &shell_cd,
    &shell_help,
    &shell_exit};

// Returns the number of built-in commands
int count_builtins()
{
    return sizeof(builtin_str) / sizeof(char *);
}

// Launches a new process to execute a command
int shell_launch(char **args)
{
    pid_t pid;
    pid_t wpid;
    int status;

    pid = fork();
    if (pid == 0)
    {
        // Child process
        if (execvp(args[0], args) == -1)
        {
            perror("shell: error executing command");
        }
    }
    else if (pid < 0)
    {
        // Error forking
        perror("shell: error forking");
    }
    else
    {
        // Parent process
        do
        {
            wpid = waitpid(pid, &status, WUNTRACED);
            if (wpid == -1)
            {
                perror("shell: error in waitpid");
                break;
            }
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }

    return 1;
}

// Built-in command: change directory
int shell_cd(char **args)
{
    if (args[1] == NULL)
    {
        fprintf(stderr, "shell: expected argument to \"cd\"\n");
    }
    else
    {
        if (chdir(args[1]) != 0)
        {
            perror("error executing cd command");
        }
    }
    return 1;
}

// Built-in command: display help information
int shell_help(char **args)
{
    int i;
    printf("Little Pink's simple shell\n");
    printf("Type program names and arguments, and hit enter.\n");
    printf("The following are built in:\n");

    for (i = 0; i < count_builtins(); i++)
    {
        printf("  %s\n", builtin_str[i]);
    }

    printf("Use the man command for information on other programs.\n");
    return 1;
}

// Built-in command: exit the shell
int shell_exit(char **args)
{
    return 0;
}

// Execute a command
int shell_execute(char **args)
{
    int i;
    if (args[0] == NULL)
    {
        // Empty command
        return 1;
    }
    for (i = 0; i < count_builtins(); i++)
    {
        if (strcmp(args[0], builtin_str[i]) == 0)
        {
            // Execute built-in command
            return (*builtin_func[i])(args);
        }
    }

    // Launch external command
    return shell_launch(args);
}

// Read a line of input from the user
char *read_line(void)
{
    int buffer_size = LINE_BUFSIZE;
    int position = 0;
    char *buffer = malloc(sizeof(char) * buffer_size);
    int ch;

    if (!buffer)
    {
        fprintf(stderr, "shell: allocation error\n");
        exit(EXIT_FAILURE);
    }

    while (1)
    {
        ch = getchar();
        if (ch == EOF)
        {
            exit(EXIT_SUCCESS);
        }
        else if (ch == '\n')
        {
            buffer[position] = '\0';
            return buffer;
        }
        else
        {
            buffer[position] = ch;
        }
        position++;

        if (position >= buffer_size)
        {
            buffer_size += LINE_BUFSIZE;
            buffer = realloc(buffer, buffer_size);
            if (!buffer)
            {
                fprintf(stderr, "shell: allocation error\n");
                exit(EXIT_FAILURE);
            }
        }
    }
}

// Split a line into tokens
char **split_lines(char *line)
{
    int buffer_size = TOKEN_BUFSIZE;
    int position = 0;
    char **tokens = malloc(buffer_size * sizeof(char *));
    char *token;

    if (!tokens)
    {
        fprintf(stderr, "shell: allocation error\n");
        exit(EXIT_FAILURE);
    }

    token = strtok(line, TOKEN_DELIM);
    while (token != NULL)
    {
        tokens[position] = token;
        position++;

        if (position >= buffer_size)
        {
            buffer_size += TOKEN_BUFSIZE;
            tokens = realloc(tokens, buffer_size * sizeof(char *));
            if (!tokens)
            {
                fprintf(stderr, "shell: allocation error\n");
                exit(EXIT_FAILURE);
            }
        }

        token = strtok(NULL, TOKEN_DELIM);
    }

    tokens[position] = NULL;
    return tokens;
}

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

// Main function
int main(int argc, char **argv)
{
    shell_loop();
    return EXIT_SUCCESS;
}
