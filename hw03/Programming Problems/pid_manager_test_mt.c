#include "pid_manager.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_THREADS 100

void *thread_func(void *arg)
{
    int pid = allocate_pid();
    if (pid == -1)
    {
        printf("Thread %ld: Failed to allocate PID\n", (long)arg);
        return NULL;
    }

    printf("Thread %ld: Allocated PID %d\n", (long)arg, pid);

    // Sleep for 1~5 seconds randomly
    int sleep_time = rand() % 5 + 1;
    sleep(sleep_time);

    release_pid(pid);
    printf("Thread %ld: Released PID %d\n", (long)arg, pid);

    return NULL;
}

int main()
{
    srand(time(NULL));
    if (allocate_map() == -1)
    {
        printf("Failed to initialize PID map.\n");
        return 1;
    }

    pthread_t threads[NUM_THREADS];

    for (long i = 0; i < NUM_THREADS; i++)
    {
        pthread_create(&threads[i], NULL, thread_func, (void *)i);
    }

    for (int i = 0; i < NUM_THREADS; i++)
    {
        pthread_join(threads[i], NULL);
    }

    printf("All threads finished.\n");
    return 0;
}
