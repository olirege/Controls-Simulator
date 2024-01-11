CC=gcc
CFLAGS=-I. -I/usr/include/postgresql
LIBS=-lpq

# Define object files
OBJ = main.o commands/commands.o commands/command_functions.o commands/command_processor.o sockets/sockets.o utils/use_logger.o

# Default target
app: $(OBJ)
	$(CC) -o $@ $^ $(LIBS)

# To obtain object files
%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

# Clean up
clean:
	rm -f $(OBJ) app

# Phony targets
.PHONY: clean
