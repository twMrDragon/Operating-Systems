#include <stdio.h>
#include <stdlib.h>

#define  PAGE_SIZE_BIT 12

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <virtual address>\n", argv[0]);
        return 1;
    }

    unsigned int address = (unsigned int)atoi(argv[1]);

    unsigned int page_number = address >> PAGE_SIZE_BIT;
    unsigned int offset = address & ((1 << PAGE_SIZE_BIT) - 1);

    printf("The address %u contains:\n", address);
    printf("page number = %u\n", page_number);
    printf("offset = %u\n", offset);

    return 0;
}
