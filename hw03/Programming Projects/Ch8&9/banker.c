#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>

#define NUM_CUSTOMERS 5
#define MAX_RESOURCES 10

int num_resources;
int *available;
int maximum[NUM_CUSTOMERS][MAX_RESOURCES];
int allocation[NUM_CUSTOMERS][MAX_RESOURCES];
int need[NUM_CUSTOMERS][MAX_RESOURCES];

pthread_mutex_t mutex;

// Utility: get minimum of two values
int min(int a, int b) {
    return (a < b) ? a : b;
}

// Initialize system with max need and available resources
void initialize_system(int argc, char *argv[]) {
    num_resources = argc - 1;
    available = malloc(sizeof(int) * num_resources);

    for (int i = 0; i < num_resources; i++) {
        available[i] = atoi(argv[i + 1]);
    }

    srand(time(NULL));
    for (int i = 0; i < NUM_CUSTOMERS; i++) {
        for (int j = 0; j < num_resources; j++) {
            maximum[i][j] = rand() % (available[j] + 1);
            allocation[i][j] = 0;
            need[i][j] = maximum[i][j];
        }
    }
}

// Check if system is in safe state
bool is_safe() {
    int work[num_resources];
    bool finish[NUM_CUSTOMERS] = { false };

    for (int i = 0; i < num_resources; i++)
        work[i] = available[i];

    while (true) {
        bool found = false;
        for (int i = 0; i < NUM_CUSTOMERS; i++) {
            if (!finish[i]) {
                bool can_finish = true;
                for (int j = 0; j < num_resources; j++) {
                    if (need[i][j] > work[j]) {
                        can_finish = false;
                        break;
                    }
                }
                if (can_finish) {
                    for (int j = 0; j < num_resources; j++)
                        work[j] += allocation[i][j];
                    finish[i] = true;
                    found = true;
                }
            }
        }
        if (!found) break;
    }

    for (int i = 0; i < NUM_CUSTOMERS; i++)
        if (!finish[i])
            return false;
    return true;
}

// Request resources
bool request_resources(int customer_id, int request[]) {
    pthread_mutex_lock(&mutex);

    for (int i = 0; i < num_resources; i++) {
        if (request[i] > need[customer_id][i] || request[i] > available[i]) {
            pthread_mutex_unlock(&mutex);
            return false;
        }
    }

    // Try allocate
    for (int i = 0; i < num_resources; i++) {
        available[i] -= request[i];
        allocation[customer_id][i] += request[i];
        need[customer_id][i] -= request[i];
    }

    if (!is_safe()) {
        // Rollback
        for (int i = 0; i < num_resources; i++) {
            available[i] += request[i];
            allocation[customer_id][i] -= request[i];
            need[customer_id][i] += request[i];
        }
        pthread_mutex_unlock(&mutex);
        return false;
    }

    pthread_mutex_unlock(&mutex);
    return true;
}

// Release resources
void release_resources(int customer_id, int release[]) {
    pthread_mutex_lock(&mutex);
    for (int i = 0; i < num_resources; i++) {
        allocation[customer_id][i] -= release[i];
        available[i] += release[i];
        need[customer_id][i] += release[i];
    }
    pthread_mutex_unlock(&mutex);
}

// Thread function
void *customer_thread(void *arg) {
    int customer_id = *(int *)arg;
    int request[MAX_RESOURCES];
    int release[MAX_RESOURCES];

    while (1) {
        sleep(rand() % 3 + 1); // 模擬時間間隔

        // 產生請求
        for (int i = 0; i < num_resources; i++) {
            request[i] = rand() % (need[customer_id][i] + 1);
        }

        if (request_resources(customer_id, request)) {
            printf("Customer %d granted: ", customer_id);
            for (int i = 0; i < num_resources; i++) printf("%d ", request[i]);
            printf("\n");

            sleep(rand() % 2 + 1); // 模擬使用資源

            for (int i = 0; i < num_resources; i++) {
                release[i] = request[i]; // 全部釋放
            }
            release_resources(customer_id, release);

            printf("Customer %d released: ", customer_id);
            for (int i = 0; i < num_resources; i++) printf("%d ", release[i]);
            printf("\n");
        } else {
            printf("Customer %d request denied\n", customer_id);
        }
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <resource1> <resource2> ...\n", argv[0]);
        return 1;
    }

    initialize_system(argc, argv);
    pthread_mutex_init(&mutex, NULL);

    pthread_t threads[NUM_CUSTOMERS];
    int ids[NUM_CUSTOMERS];

    for (int i = 0; i < NUM_CUSTOMERS; i++) {
        ids[i] = i;
        pthread_create(&threads[i], NULL, customer_thread, &ids[i]);
    }

    for (int i = 0; i < NUM_CUSTOMERS; i++) {
        pthread_join(threads[i], NULL);
    }

    pthread_mutex_destroy(&mutex);
    free(available);
    return 0;
}
