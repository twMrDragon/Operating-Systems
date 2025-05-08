CC = gcc
CFLAGS = -Wall -pthread

# Executables
TARGETS = address fibonacci_thread pid_manager_test_mt farmer_bridge

# Build rules
all: $(TARGETS)

address: address.c
	$(CC) $(CFLAGS) -o address address.c

fibonacci_thread: fibonacci_thread.c
	$(CC) $(CFLAGS) -o fibonacci_thread fibonacci_thread.c

pid_manager_test_mt: pid_manager_test_mt.c pid_manager.c pid_manager.h
	$(CC) $(CFLAGS) -o pid_manager_test_mt pid_manager_test_mt.c pid_manager.c

farmer_bridge: farmer_bridge.c
	$(CC) $(CFLAGS) -o farmer_bridge farmer_bridge.c

clean:
	rm -f $(TARGETS)
