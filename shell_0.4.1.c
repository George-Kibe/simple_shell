#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define BUFFER_SIZE 1024
#define PROMPT "$ "

int execute(char **args, char **envp);
char *read_line(void);
char **split_line(char *line);
int shell_exit(char **args);
int shell_env(char **envp);

int main(int argc, char **argv, char **envp) {
    char *line;
    char **args;
    int status;

    do {
        printf(PROMPT);
        line = read_line();
        args = split_line(line);
        status = execute(args, envp);

        free(line);
        free(args);
    } while (status);

    return EXIT_SUCCESS;
}

char *read_line(void) {
    static char line[BUFFER_SIZE];
    static int pos = 0;

    while (1) {
        char c = getchar();

        if (c == EOF || c == '\n') {
            line[pos] = '\0';
            pos = 0;
            return line;
        } else {
            line[pos] = c;
        }

        if (++pos >= BUFFER_SIZE) {
            fprintf(stderr, "Error: command line too long.\n");
            exit(EXIT_FAILURE);
        }
    }
}

char **split_line(char *line) {
    static char *tokens[BUFFER_SIZE];
    static int pos = 0;

    char *token;

    token = strtok(line, " ");
    while (token != NULL) {
        tokens[pos++] = token;
        token = strtok(NULL, " ");
    }

    tokens[pos] = NULL;
    return tokens;
}

int execute(char **args, char **envp) {
    pid_t pid, wpid;
    int status;

    if (args[0] == NULL) {
        return 1;
    }

    if (strcmp(args[0], "exit") == 0) {
        return shell_exit(args);
    }

    if (strcmp(args[0], "env") == 0) {
        return shell_env(envp);
    }

    pid = fork();
    if (pid == 0) {
        if (execve(args[0], args, envp) == -1) {
            fprintf(stderr, "Error: command not found.\n");
        }
        exit(EXIT_FAILURE);
    } else if (pid < 0) {
        fprintf(stderr, "Error: forking failed.\n");
    } else {
        do {
            wpid = waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }

    return 1;
}

int shell_exit(char **args) {
    if (args[1] != NULL) {
        int status = atoi(args[1]);
        exit(status);
    } else {
        exit(EXIT_SUCCESS);
    }
}

int shell_env(char **envp) {
    for (int i = 0; envp[i] != NULL; i++) {
        printf("%s\n", envp[i]);
    }

    return 1;
}

