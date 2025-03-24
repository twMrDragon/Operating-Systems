// pid_manager_test.c
#include "pid_manager.h"
#include <stdio.h>

int main() {
    if (allocate_map() == -1) {
        printf("Failed to initialize PID map.\n");
        return 1;
    }

    int pid1 = allocate_pid();
    int pid2 = allocate_pid();
    int pid3 = allocate_pid();
    
    printf("Allocated PIDs: %d, %d, %d\n", pid1, pid2, pid3);
    
    release_pid(pid2);
    printf("Released PID: %d\n", pid2);
    
    int pid4 = allocate_pid();
    printf("New allocated PID (should reuse %d): %d\n", pid2, pid4);
    
    return 0;
}
