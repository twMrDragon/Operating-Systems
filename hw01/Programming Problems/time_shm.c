#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <string.h>

#define SHM_NAME "/shm_time"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <command> [args...]\n", argv[0]);
        return 1;
    }

    int shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    ftruncate(shm_fd, sizeof(struct timeval));
    struct timeval *start_time = mmap(0, sizeof(struct timeval), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);

    pid_t pid = fork();
    if (pid == 0) {  // Child process
        gettimeofday(start_time, NULL);
        execvp(argv[1], &argv[1]);
        perror("execvp failed");
        exit(1);
    } else {  // Parent process
        wait(NULL);
        struct timeval end_time;
        gettimeofday(&end_time, NULL);
        double elapsed = (end_time.tv_sec - start_time->tv_sec) + (end_time.tv_usec - start_time->tv_usec) / 1e6;
        printf("Elapsed time (shared memory): %.6f seconds\n", elapsed);
        shm_unlink(SHM_NAME);
    }
    return 0;
}
