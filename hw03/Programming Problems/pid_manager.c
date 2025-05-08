// pid_manager.c
#include "pid_manager.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#define PID_RANGE (MAX_PID - MIN_PID + 1)

static pthread_mutex_t pid_mutex = PTHREAD_MUTEX_INITIALIZER;

int pid_map[PID_RANGE]; // Bitmap to track PID availability

// Initialize the PID map
int allocate_map(void)
{
    memset(pid_map, 0, sizeof(pid_map));
    return 1; // Always successful in this implementation
}

// Allocate a PID
int allocate_pid(void)
{
    pthread_mutex_lock(&pid_mutex);
    for (int i = 0; i < PID_RANGE; i++)
    {
        if (pid_map[i] == 0)
        {
            pid_map[i] = 1;
            pthread_mutex_unlock(&pid_mutex);
            return i + MIN_PID;
        }
    }
    pthread_mutex_unlock(&pid_mutex);
    return -1;
}

// Release a PID
void release_pid(int pid)
{
    if (pid < MIN_PID || pid > MAX_PID)
        return;
    pthread_mutex_lock(&pid_mutex);
    pid_map[pid - MIN_PID] = 0;
    pthread_mutex_unlock(&pid_mutex);
}
