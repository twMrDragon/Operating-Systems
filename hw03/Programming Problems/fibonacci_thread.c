#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

long *fib_sequence;
int n;

int current_index = -1;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

void *generate_fibonacci(void *arg)
{
    if (n > 0) {
        pthread_mutex_lock(&mutex);
        fib_sequence[0] = 0;
        current_index = 0;
        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&mutex);
    }

    if (n > 1) {
        pthread_mutex_lock(&mutex);
        fib_sequence[1] = 1;
        current_index = 1;
        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&mutex);
    }

    for (int i = 2; i < n; i++) {
        pthread_mutex_lock(&mutex);
        fib_sequence[i] = fib_sequence[i - 1] + fib_sequence[i - 2];
        current_index = i;
        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&mutex);
        // 模擬計算時間
        usleep(50000);  // 0.1 秒
    }

    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    if (argc != 2) {
        printf("Usage: %s <number of Fibonacci numbers to generate>\n", argv[0]);
        return 1;
    }

    n = atoi(argv[1]);
    if (n <= 0) {
        printf("Please enter a positive integer greater than 0.\n");
        return 1;
    }

    fib_sequence = (long *)malloc(sizeof(long) * n);
    if (fib_sequence == NULL) {
        perror("Memory allocation failed");
        return 1;
    }

    pthread_t tid;
    if (pthread_create(&tid, NULL, generate_fibonacci, NULL) != 0) {
        perror("Failed to create thread");
        free(fib_sequence);
        return 1;
    }

    printf("Fibonacci sequence (%d numbers):\n", n);
    for (int i = 0; i < n; i++) {
        pthread_mutex_lock(&mutex);
        while (current_index < i)
            pthread_cond_wait(&cond, &mutex);
        printf("fib[%d] = %ld\n", i, fib_sequence[i]);
        pthread_mutex_unlock(&mutex);
    }
    printf("\n");

    pthread_join(tid, NULL);
    free(fib_sequence);
    return 0;
}
