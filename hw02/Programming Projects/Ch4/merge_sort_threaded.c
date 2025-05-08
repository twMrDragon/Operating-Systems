#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define MAX 100

int arr[MAX];
int size;
int sorted[MAX];

typedef struct {
    int left;
    int right;
} Range;

void merge(int left, int mid, int right) {
    int i = left;
    int j = mid + 1;
    int k = left;

    while (i <= mid && j <= right) {
        if (arr[i] <= arr[j])
            sorted[k++] = arr[i++];
        else
            sorted[k++] = arr[j++];
    }

    while (i <= mid)
        sorted[k++] = arr[i++];
    while (j <= right)
        sorted[k++] = arr[j++];

    for (int i = left; i <= right; i++)
        arr[i] = sorted[i];
}

void merge_sort(int left, int right) {
    if (left < right) {
        int mid = (left + right) / 2;
        merge_sort(left, mid);
        merge_sort(mid + 1, right);
        merge(left, mid, right);
    }
}

// Thread function for sorting a range using merge sort
void* thread_merge_sort(void* arg) {
    Range* range = (Range*)arg;
    merge_sort(range->left, range->right);
    free(range);
    pthread_exit(NULL);
}

// Thread function for merging two halves
void* thread_merge(void* arg) {
    int mid = (size - 1) / 2;
    merge(0, mid, size - 1);
    pthread_exit(NULL);
}

// Function to load integers from a text file
int load_from_file(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("無法開啟檔案");
        return 0;
    }

    fscanf(file, "%d", &size);  // Read the size of the array
    if (size > MAX || size <= 0) {
        printf("Invalid size in file.\n");
        fclose(file);
        return 0;
    }

    for (int i = 0; i < size; i++) {
        fscanf(file, "%d", &arr[i]);
    }

    fclose(file);
    return 1;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("使用方式：%s <檔案名稱>\n", argv[0]);
        return 1;
    }

    const char* filename = argv[1];
    if (!load_from_file(filename)) {
        return 1;
    }

    pthread_t tid1, tid2, tid3;

    // Prepare arguments for threads
    Range* left_range = malloc(sizeof(Range));
    left_range->left = 0;
    left_range->right = (size - 1) / 2;

    Range* right_range = malloc(sizeof(Range));
    right_range->left = (size - 1) / 2 + 1;
    right_range->right = size - 1;

    // Launch two sorting threads
    pthread_create(&tid1, NULL, thread_merge_sort, left_range);
    pthread_create(&tid2, NULL, thread_merge_sort, right_range);

    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);

    // Merge the two sorted halves using another thread
    pthread_create(&tid3, NULL, thread_merge, NULL);
    pthread_join(tid3, NULL);

    // Output sorted array
    printf("Sorted array:\n");
    for (int i = 0; i < size; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");

    return 0;
}
