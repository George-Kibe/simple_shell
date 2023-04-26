#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>

extern char **environ;

#define BUFFER_SIZE 1024

/**
 * main - Entry point
 *
 * Return: Always 0
 */
int main(void)
{
    char *buffer;
    size_t bufsize = BUFFER_SIZE;
    int status;
    pid_t pid;
    char *args[] = {NULL};

    buffer = malloc(sizeof(char) * bufsize);
    if (buffer == NULL)
    {
        perror("Unable to allocate buffer");
        exit(1);
    }

    while (1)
    {
        printf("$ ");
        getline(&buffer, &bufsize, stdin);
        buffer[strlen(buffer) - 1] = '\0'; // Remove trailing newline

        pid = fork();
        if (pid == 0)
        {
            if (execve(buffer, args, NULL) == -1)
            {
                perror("Error: ");
            }
            exit(EXIT_FAILURE);
        }
        else if (pid < 0)
        {
            perror("Error: ");
        }
        else
        {
            wait(&status);
        }
    }

    free(buffer);
    return (0);
}
