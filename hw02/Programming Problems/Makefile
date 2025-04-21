# Makefile for Fibonacci and PID Manager projects

# Compiler settings
CC = gcc
CFLAGS = -Wall -pthread

# Output files
FIBO_EXEC = fibonacci_thread
PID_EXEC = pid_manager_test_mt

# Source files
FIBO_SRC = fibonacci_thread.c
PID_SRC = pid_manager.c pid_manager_test_mt.c

# Header files
PID_HEADERS = pid_manager.h

# Default target to build both executables
all: $(FIBO_EXEC) $(PID_EXEC)

# Rule to build fibonacci_thread executable
$(FIBO_EXEC): $(FIBO_SRC)
	$(CC) $(CFLAGS) $(FIBO_SRC) -o $(FIBO_EXEC)

# Rule to build pid_manager_test_mt executable
$(PID_EXEC): $(PID_SRC) $(PID_HEADERS)
	$(CC) $(CFLAGS) $(PID_SRC) -o $(PID_EXEC)

# Clean target to remove compiled files
clean:
	rm -f $(FIBO_EXEC) $(PID_EXEC)

# Phony targets to avoid conflicts with file names
.PHONY: all clean
