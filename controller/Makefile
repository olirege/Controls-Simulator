CC=gcc

CFLAGS=-I/usr/include/postgresql -pthread
LDFLAGS =-lpq -lcurl -pthread

DEPS = sockets/sockets.h commands/command_functions.h commands/command_processor.h commands/commands.h utils/utils.h queue/queue.h memory/memory.h 

OBJ = controller.o sockets/sockets.o commands/command_functions.o commands/command_processor.o commands/commands.o utils/use_request.o utils/use_db.o utils/use_logger.o queue/queue.o memory/memory.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

controller: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)