#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#define BUFFER_SIZE 4096

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <source file> <destination file>\n", argv[0]);
        return 1;
    }

    const char *src = argv[1];
    const char *dest = argv[2];

    int src_fd = open(src, O_RDONLY);
    if (src_fd == -1) {
        perror("Error opening source file");
        return 1;
    }

    int dest_fd = open(dest, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (dest_fd == -1) {
        perror("Error opening destination file");
        close(src_fd);
        return 1;
    }

    char buffer[BUFFER_SIZE];
    ssize_t bytes_read;
    while ((bytes_read = read(src_fd, buffer, BUFFER_SIZE)) > 0) {
        if (write(dest_fd, buffer, bytes_read) != bytes_read) {
            perror("Error writing to destination file");
            close(src_fd);
            close(dest_fd);
            return 1;
        }
    }

    if (bytes_read == -1)
        perror("Error reading from source file");
    
    close(src_fd);
    close(dest_fd);
    printf("File copied successfully.\n");
    return 0;
}
