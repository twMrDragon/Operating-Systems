#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// Pointer to shared array (type: long)
long *fib_sequence;
// Number of Fibonacci numbers to generate
int n;

// Thread function to generate the sequence
void *generate_fibonacci(void *arg)
{
    if (n > 0)
        fib_sequence[0] = 0;
    if (n > 1)
        fib_sequence[1] = 1;

    for (int i = 2; i < n; i++)
    {
        fib_sequence[i] = fib_sequence[i - 1] + fib_sequence[i - 2];
    }

    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: %s <number of Fibonacci numbers to generate>\n", argv[0]);
        return 1;
    }

    n = atoi(argv[1]);

    if (n <= 0)
    {
        printf("Please enter a positive integer greater than 0.\n");
        return 1;
    }

    // Allocate memory for the sequence (as long)
    fib_sequence = (long *)malloc(sizeof(long) * n);
    if (fib_sequence == NULL)
    {
        perror("Memory allocation failed");
        return 1;
    }

    pthread_t tid;

    // Create the child thread
    if (pthread_create(&tid, NULL, generate_fibonacci, NULL) != 0)
    {
        perror("Failed to create thread");
        free(fib_sequence);
        return 1;
    }

    // Wait for the thread to finish
    pthread_join(tid, NULL);

    // Output the result
    printf("Fibonacci sequence (%d numbers):\n", n);
    for (int i = 0; i < n; i++)
    {
        printf("%ld ", fib_sequence[i]);
    }
    printf("\n");

    // Free memory
    free(fib_sequence);
    return 0;
}
