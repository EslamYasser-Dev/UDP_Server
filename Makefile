# Compiler
CC=gcc

# Compiler flags
CFLAGS=-Iinclude -Wall -g

# Target executable
TARGET=udp_server

# Source files and corresponding object files
SRCS=src/main.c src/ffplay_handler.c
OBJS=$(SRCS:.c=.o)

# Default target
all: $(TARGET)

# Linking the target executable
$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(OBJS)

# Compiling source files into object files
%.o: %.c include/config.h include/ffplay_handler.h
	$(CC) $(CFLAGS) -c $< -o $@

# Clean target to remove compiled files
clean:
	rm -f $(TARGET) src/*.o
