#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#define MAX_INPUT 1024
#define MAX_ARGS 64
#define HISTORY_SIZE 10

char *history[HISTORY_SIZE];
int history_count = 0;

void add_to_history(char *cmd) {
    if (history_count < HISTORY_SIZE) {
        history[history_count++] = strdup(cmd);
    } else {
        free(history[0]);
        for (int i = 1; i < HISTORY_SIZE; i++) {
            history[i - 1] = history[i];
        }
        history[HISTORY_SIZE - 1] = strdup(cmd);
    }
}

void print_history() {
    for (int i = 0; i < history_count; i++) {
        printf("%d: %s\n", i + 1, history[i]);
    }
}

void executeCommand(char *const *args, int pipefd_1[2], int pipefd_2[2]) {
    int pid = fork();
    if (pid == 0) {
        if (pipefd_1 != NULL) {
            close(pipefd_1[1]);
            dup2(pipefd_1[0], STDIN_FILENO);
            close(pipefd_1[0]);
        }
        if (pipefd_2 != NULL) {
            close(pipefd_2[0]);
            dup2(pipefd_2[1], STDOUT_FILENO);
            close(pipefd_2[1]);
        }
        execvp(args[0], args);
        perror("execvp");
        exit(EXIT_FAILURE);
    } else {
        waitpid(pid, NULL, 0);
    }
}

void executeCommandEntry(char *const *args, int pipefd_parent[2]) {
    int pipefd_child[2];
    int pipe_index = -1;
    char *args1[MAX_ARGS] = {NULL}, *args2[MAX_ARGS] = {NULL};
    int i = 0;

    while (args[i++] != NULL)
        ;

    for (int j = 0; j < i - 1; j++) {
        if (strcmp(args[j], "|") == 0) {
            pipe_index = j;
            break;
        }
    }

    if (pipe_index != -1) {
        pipe(pipefd_child);
        memcpy(args1, args, pipe_index * sizeof(char *));
        args1[pipe_index] = NULL;
        memcpy(args2, &args[pipe_index + 1],
               (i - pipe_index - 1) * sizeof(char *));
        args2[i - pipe_index - 1] = NULL;

        if (pipefd_parent == NULL) {
            executeCommand(args1, NULL, pipefd_child);
        } else {
            executeCommand(args1, pipefd_parent, pipefd_child);
        }

        close(pipefd_child[1]); // close write end of pipe
        executeCommandEntry(args2, pipefd_child);
        close(pipefd_child[0]); // close read end of pipe
    } else {
        if (pipefd_parent == NULL) {
            executeCommand(args, NULL, NULL);
        } else {
            executeCommand(args, pipefd_parent, NULL);
        }
    }
}

void parse_execute(char *input) {
    add_to_history(input);
    char *args[MAX_ARGS];
    char *token;
    int i = 0, j = 0, pipe_flag = 0;
    char *input_file = NULL, *output_file = NULL;

    int saved_stdin = dup(STDIN_FILENO);   // Save original stdin
    int saved_stdout = dup(STDOUT_FILENO); // Save original stdout

    while (input[j] != '\0') {
        while (input[j] == ' ') {
            j++;
        }
        if (input[j] == '\0') {
            break;
        }
        if (input[j] == '"') {
            j++;
            token = &input[j];
            while (input[j] != '"' && input[j] != '\0') {
                j++;
            }
            if (input[j] != '\0') {
                input[j++] = '\0';
            }
            args[i++] = token;
            continue;
        }
        if (input[j] == '>') {
            j++;
            while (input[j] == ' ') {
                j++;
            }
            output_file = &input[j];
            while (input[j] != ' ' && input[j] != '\0') {
                j++;
            }
            if (input[j] != '\0') {
                input[j++] = '\0';
            }
            int fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd < 0) {
                perror("open");
                dup2(saved_stdin, STDIN_FILENO);   // Restore stdin
                dup2(saved_stdout, STDOUT_FILENO); // Restore stdout
                close(saved_stdin);
                close(saved_stdout);
                return;
            }
            dup2(fd, STDOUT_FILENO);
            close(fd);
            continue;
        }
        if (input[j] == '<') {
            j++;
            while (input[j] == ' ') {
                j++;
            }
            input_file = &input[j];
            while (input[j] != ' ' && input[j] != '\0') {
                j++;
            }
            if (input[j] != '\0') {
                input[j++] = '\0';
            }
            int fd = open(input_file, O_RDONLY);
            if (fd < 0) {
                perror("open");
                dup2(saved_stdin, STDIN_FILENO);   // Restore stdin
                dup2(saved_stdout, STDOUT_FILENO); // Restore stdout
                close(saved_stdin);
                close(saved_stdout);
                return;
            }
            dup2(fd, STDIN_FILENO);
            close(fd);
            continue;
        }
        token = &input[j];
        pipe_flag = 0;
        while (input[j] != ' ' && input[j] != '\0') {
            if (input[j] == '|') {
                pipe_flag = 1;
                break;
            }
            j++;
        }
        if (input[j] != '\0') {
            input[j++] = '\0';
        }
        args[i++] = token;
        if (pipe_flag || input[j] == '|') {
            if (input[j] == '|') {
                j++;
            }
            args[i++] = "|";
        }
    }
    args[i] = NULL;

    if (i == 0) {
        return;
    }
    if (strcmp(args[0], "exit") == 0) {
        exit(0);
    }
    if (strcmp(args[0], "history") == 0) {
        print_history();
        return;
    }

    // printf("Arguments:\n");
    // for (int k = 0; args[k] != NULL; k++) {
    //     printf("args[%d]: %s\n", k, args[k]);
    // }

    executeCommandEntry(args, NULL);

    // Restore original stdin and stdout
    dup2(saved_stdin, STDIN_FILENO);
    dup2(saved_stdout, STDOUT_FILENO);
    close(saved_stdin);
    close(saved_stdout);
}

int main() {
    char input[MAX_INPUT];
    while (1) {
        printf("unixshell> ");
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        input[strcspn(input, "\n")] = 0;
        parse_execute(input);
    }
    return 0;
}