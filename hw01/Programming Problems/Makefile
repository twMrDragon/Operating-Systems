CC = gcc
CFLAGS = -Wall -lrt

all: FileCopy time_shm time_pipe

FileCopy: FileCopy.c
	$(CC) -o FileCopy FileCopy.c $(CFLAGS)

# Compile shared memory version
time_shm: time_shm.c
	$(CC) -o time_shm time_shm.c $(CFLAGS)

# Compile pipe version
time_pipe: time_pipe.c
	$(CC) -o time_pipe time_pipe.c $(CFLAGS)

clean:
	rm -f FileCopy time_shm time_pipe