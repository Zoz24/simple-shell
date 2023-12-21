CC = gcc
CFLAGS = -Wall -g
OBJS = main.o shell_core.o builtins.o utilities.o

# Name of your executable
EXEC = myshell

# Default target
all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) -o $(EXEC) $(OBJS)

main.o: main.c shell_core.h
	$(CC) $(CFLAGS) -c main.c

shell_core.o: shell_core.c shell_core.h builtins.h utilities.h shared.h
	$(CC) $(CFLAGS) -c shell_core.c

builtins.o: builtins.c builtins.h shared.h
	$(CC) $(CFLAGS) -c builtins.c

utilities.o: utilities.c utilities.h
	$(CC) $(CFLAGS) -c utilities.c

# Clean up
clean:
	rm -f $(OBJS) $(EXEC)

# Phony targets
.PHONY: all clean
