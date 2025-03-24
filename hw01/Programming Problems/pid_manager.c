// pid_manager.c
#include "pid_manager.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PID_RANGE (MAX_PID - MIN_PID + 1)

int pid_map[PID_RANGE]; // Bitmap to track PID availability

// Initialize the PID map
int allocate_map(void) {
    memset(pid_map, 0, sizeof(pid_map));
    return 1; // Always successful in this implementation
}

// Allocate a PID
int allocate_pid(void) {
    for (int i = 0; i < PID_RANGE; i++) {
        if (pid_map[i] == 0) { // Found an available PID
            pid_map[i] = 1; // Mark as used
            return i + MIN_PID;
        }
    }
    return -1; // No available PIDs
}

// Release a PID
void release_pid(int pid) {
    if (pid < MIN_PID || pid > MAX_PID) return;
    pid_map[pid - MIN_PID] = 0; // Mark as available
}
