CC = gcc
CFLAGS = -Wall -pthread

# Targets
TARGETS = banker memory_manager

# Source files
SRCS_banker = banker.c
OBJS_banker = $(SRCS_banker:.c=.o)

SRCS_memory_manager = memory_manager.c
OBJS_memory_manager = $(SRCS_memory_manager:.c=.o)

all: $(TARGETS)

banker: $(OBJS_banker)
	$(CC) $(CFLAGS) -o $@ $^

memory_manager: $(OBJS_memory_manager)
	$(CC) -Wall -o $@ $^
%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(TARGETS) *.o
