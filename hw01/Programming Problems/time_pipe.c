#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <command> [args...]\n", argv[0]);
        return 1;
    }

    int fd[2];
    pipe(fd);
    struct timeval start_time;
    pid_t pid = fork();
    
    if (pid == 0) {  // Child process
        close(fd[0]);
        gettimeofday(&start_time, NULL);
        write(fd[1], &start_time, sizeof(start_time));
        close(fd[1]);
        execvp(argv[1], &argv[1]);
        perror("execvp failed");
        exit(1);
    } else {  // Parent process
        close(fd[1]);
        wait(NULL);
        struct timeval end_time;
        read(fd[0], &start_time, sizeof(start_time));
        gettimeofday(&end_time, NULL);
        double elapsed = (end_time.tv_sec - start_time.tv_sec) + (end_time.tv_usec - start_time.tv_usec) / 1e6;
        printf("Elapsed time (pipe): %.6f seconds\n", elapsed);
        close(fd[0]);
    }
    return 0;
}
