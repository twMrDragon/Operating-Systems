#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

#define BUFFER_SIZE 1024

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <source file> <destination file>\n", argv[0]);
        return 1;
    }

    int pipe_fd[2];
    if (pipe(pipe_fd) == -1) {
        perror("pipe failed");
        return 1;
    }

    pid_t pid = fork();
    if (pid < 0) {
        perror("fork failed");
        return 1;
    }

    if (pid == 0) { // Child process
        close(pipe_fd[1]); // Close write end
        
        int dest_fd = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0666);
        if (dest_fd == -1) {
            perror("Failed to open destination file");
            return 1;
        }
        
        char buffer[BUFFER_SIZE];
        ssize_t bytes_read;
        while ((bytes_read = read(pipe_fd[0], buffer, BUFFER_SIZE)) > 0) {
            write(dest_fd, buffer, bytes_read);
        }
        
        close(dest_fd);
        close(pipe_fd[0]);
        return 0;
    } else { // Parent process
        close(pipe_fd[0]); // Close read end
        
        int src_fd = open(argv[1], O_RDONLY);
        if (src_fd == -1) {
            perror("Failed to open source file");
            return 1;
        }
        
        char buffer[BUFFER_SIZE];
        ssize_t bytes_read;
        while ((bytes_read = read(src_fd, buffer, BUFFER_SIZE)) > 0) {
            write(pipe_fd[1], buffer, bytes_read);
        }
        
        close(src_fd);
        close(pipe_fd[1]);
        wait(NULL);
    }
    return 0;
}
