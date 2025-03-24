CC = gcc
CFLAGS = -Wall -lrt

all: FileCopy time_shm time_pipe filecopy_pipe pid_test

FileCopy: FileCopy.c
	$(CC) -o FileCopy FileCopy.c $(CFLAGS)

# Compile shared memory version
time_shm: time_shm.c
	$(CC) -o time_shm time_shm.c $(CFLAGS)

# Compile pipe version
time_pipe: time_pipe.c
	$(CC) -o time_pipe time_pipe.c $(CFLAGS)

filecopy_pipe: filecopy_pipe.c
	$(CC) -o filecopy_pipe filecopy_pipe.c $(CFLAGS)

# Compile pid_manager test
pid_test: pid_manager_test.c pid_manager.c
	$(CC) -o pid_test pid_manager_test.c pid_manager.c $(CFLAGS)

clean:
	rm -f FileCopy time_shm time_pipe filecopy_pipe pid_test