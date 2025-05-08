#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_NORTH 5
#define NUM_SOUTH 5

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_north = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_south = PTHREAD_COND_INITIALIZER;

int northbound = 0;
int southbound = 0;
int on_bridge = 0;

void* north_farmer(void* arg) {
    pthread_mutex_lock(&mutex);

    while (southbound > 0 || on_bridge > 0) {
        pthread_cond_wait(&cond_north, &mutex);
    }

    northbound++;
    on_bridge++;
    printf("Northbound farmer %ld is crossing the bridge\n", (long)arg);

    pthread_mutex_unlock(&mutex);

    // Simulate crossing the bridge
    sleep(rand() % 3 + 1);

    pthread_mutex_lock(&mutex);
    on_bridge--;
    northbound--;

    if (northbound == 0) {
        pthread_cond_broadcast(&cond_south);
    } else {
        pthread_cond_signal(&cond_north);
    }

    pthread_mutex_unlock(&mutex);
    return NULL;
}

void* south_farmer(void* arg) {
    pthread_mutex_lock(&mutex);

    while (northbound > 0 || on_bridge > 0) {
        pthread_cond_wait(&cond_south, &mutex);
    }

    southbound++;
    on_bridge++;
    printf("Southbound farmer %ld is crossing the bridge\n", (long)arg);

    pthread_mutex_unlock(&mutex);

    // Simulate crossing the bridge
    sleep(rand() % 3 + 1);

    pthread_mutex_lock(&mutex);
    on_bridge--;
    southbound--;

    if (southbound == 0) {
        pthread_cond_broadcast(&cond_north);
    } else {
        pthread_cond_signal(&cond_south);
    }

    pthread_mutex_unlock(&mutex);
    return NULL;
}

int main() {
    pthread_t threads[NUM_NORTH + NUM_SOUTH];
    srand(time(NULL));

    // Create northbound farmer threads
    for (long i = 0; i < NUM_NORTH; i++) {
        pthread_create(&threads[i], NULL, north_farmer, (void*)i);
    }

    // Create southbound farmer threads
    for (long i = 0; i < NUM_SOUTH; i++) {
        pthread_create(&threads[NUM_NORTH + i], NULL, south_farmer, (void*)i);
    }

    // Join all threads
    for (int i = 0; i < NUM_NORTH + NUM_SOUTH; i++) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}
