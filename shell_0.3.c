
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

extern char **environ;
#define BUFFER_SIZE 1024

/**
 * prompt - displays the prompt
 */
void prompt(void)
{
    printf("$ ");
}

/**
 * read_command - reads the command from the user input
 * @buffer: pointer to the buffer to store the command
 * Return: number of characters read
 */
int read_command(char *buffer)
{
    int n;

    n = getline(&buffer, &(size_t){BUFFER_SIZE}, stdin);
    if (n == -1)
        exit(EXIT_SUCCESS);

    return (n);
}

/**
 * execute_command - executes the command using execve
 * @command: pointer to the command string
 * @args: pointer to an array of arguments
 */
void execute_command(char *command, char **args)
{
    if (execvp(command, args) == -1)
    {
        perror("Error");
        exit(EXIT_FAILURE);
    }
}

/**
 * main - entry point of the program
 */
int main(void)
{
    char buffer[BUFFER_SIZE];
    char *args[BUFFER_SIZE];
    int n;

    while (1)
    {
        prompt();
        n = read_command(buffer);
        buffer[n - 1] = '\0'; // replace newline with null terminator

        int i = 0;
        args[i] = strtok(buffer, " ");
        while (args[i] != NULL)
        {
            i++;
            args[i] = strtok(NULL, " ");
        }
        args[i] = NULL;

        if (strcmp(args[0], "exit") == 0)
        {
            exit(EXIT_SUCCESS);
        }

        if (strcmp(args[0], "env") == 0)
        {
            char **envp = environ;
            while (*envp)
            {
                printf("%s\n", *envp++);
            }
            continue;
        }

        char command[BUFFER_SIZE];
        sprintf(command, "/bin/%s", args[0]); // add /bin/ prefix to command

        if (access(command, X_OK) == -1)
        {
            printf("%s: command not found\n", args[0]);
            continue;
        }

        if (fork() == 0)
        {
            execute_command(command, args);
        }
        else
        {
            wait(NULL);
        }
    }

    return (0);
}
