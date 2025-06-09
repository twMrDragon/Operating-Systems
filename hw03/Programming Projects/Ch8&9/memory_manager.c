#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Block {
    int start;
    int size;
    int allocated;  // 0 = free, 1 = allocated
    int id;         // for tracking allocated blocks
    struct Block *next;
} Block;

Block *memory = NULL;
int id_counter = 1;

void init_memory(int size) {
    memory = (Block *)malloc(sizeof(Block));
    memory->start = 0;
    memory->size = size;
    memory->allocated = 0;
    memory->id = 0;
    memory->next = NULL;
}

void print_status() {
    Block *curr = memory;
    while (curr) {
        printf("[%s] Start: %d, Size: %d", 
            curr->allocated ? "ALLOCATED" : "FREE",
            curr->start, curr->size);
        if (curr->allocated)
            printf(", ID: %d", curr->id);
        printf("\n");
        curr = curr->next;
    }
}

void request_memory(int size) {
    Block *curr = memory;

    while (curr) {
        if (!curr->allocated && curr->size >= size) {
            if (curr->size == size) {
                curr->allocated = 1;
                curr->id = id_counter++;
            } else {
                // Split the block
                Block *new_block = (Block *)malloc(sizeof(Block));
                new_block->start = curr->start + size;
                new_block->size = curr->size - size;
                new_block->allocated = 0;
                new_block->id = 0;
                new_block->next = curr->next;

                curr->size = size;
                curr->allocated = 1;
                curr->id = id_counter++;
                curr->next = new_block;
            }
            printf("Allocated at address %d with ID %d\n", curr->start, curr->id);
            return;
        }
        curr = curr->next;
    }

    printf("ERROR: Not enough memory to allocate %d units\n", size);
}

void release_memory(int id) {
    Block *curr = memory;
    int found = 0;
    while (curr) {
        if (curr->allocated && curr->id == id) {
            curr->allocated = 0;
            curr->id = 0;
            found = 1;
            printf("Released memory with ID %d\n", id);
            break;
        }
        curr = curr->next;
    }

    if (!found) {
        printf("ERROR: No allocated block with ID %d\n", id);
        return;
    }
}

void compact_memory() {
    Block *curr = memory;
    Block *prev = NULL;
    int next_start = 0;

    Block *new_head = NULL;
    Block *tail = NULL;

    // Collect allocated blocks first
    while (curr) {
        if (curr->allocated) {
            Block *new_block = (Block *)malloc(sizeof(Block));
            new_block->start = next_start;
            new_block->size = curr->size;
            new_block->allocated = 1;
            new_block->id = curr->id;
            new_block->next = NULL;

            next_start += new_block->size;

            if (!new_head) new_head = new_block;
            else tail->next = new_block;

            tail = new_block;
        }
        curr = curr->next;
    }

    // Add one free block if space remains
    int total_size = 0;
    for (Block *temp = memory; temp; temp = temp->next) {
        total_size += temp->size;
    }
    if (next_start < total_size) {
        Block *free_block = (Block *)malloc(sizeof(Block));
        free_block->start = next_start;
        free_block->size = total_size - next_start;
        free_block->allocated = 0;
        free_block->id = 0;
        free_block->next = NULL;
        if (!new_head) new_head = free_block;
        else tail->next = free_block;
    }

    // Free old list
    curr = memory;
    while (curr) {
        Block *temp = curr;
        curr = curr->next;
        free(temp);
    }

    memory = new_head;
    printf("Memory compacted.\n");
}

void print_menu_options() {
    printf("\nAvailable commands:\n");
    printf("  REQUEST <size>   - Allocate memory block of given size\n");
    printf("  RELEASE <id>     - Release allocated block with given ID\n");
    printf("  COMPACT          - Compact free memory into one block\n");
    printf("  STATUS           - Show current memory usage\n");
    printf("  EXIT             - Quit program\n");
}

void menu_loop() {
    char command[32];
    int value;

    print_menu_options();

    while (1) {
        printf("\n> ");
        int result = scanf("%s", command);
        if (result == EOF) break;

        int known = 1;

        if (strcmp(command, "REQUEST") == 0) {
            scanf("%d", &value);
            request_memory(value);
        } else if (strcmp(command, "RELEASE") == 0) {
            scanf("%d", &value);
            release_memory(value);
        } else if (strcmp(command, "COMPACT") == 0) {
            compact_memory();
        } else if (strcmp(command, "STATUS") == 0) {
            print_status();
        } else if (strcmp(command, "EXIT") == 0) {
            break;
        } else {
            known = 0;
            printf("Unknown command.\n");
            print_menu_options();
        }

        // 清除輸入緩衝區避免 REQ 10 類型指令導致第二次錯誤
        if (!known) {
            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF);
        }
    }
}


int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <memory_size>\n", argv[0]);
        exit(1);
    }

    int mem_size = atoi(argv[1]);
    if (mem_size <= 0) {
        fprintf(stderr, "Memory size must be positive.\n");
        return 1;
    }

    init_memory(mem_size);
    printf("Initialized memory with size %d\n", mem_size);
    menu_loop();

    return 0;
}
