# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Iinclude

# Linker flags
LDFLAGS = -Llib -lraylib -framework CoreFoundation -framework IOKit -framework Cocoa

# Source files
SRCS = main.c $(wildcard src/*.c)

# Object files
OBJS = $(SRCS:.c=.o)

# Executable name
TARGET = game

# Default rule
all: $(TARGET)

# Rule to build the target executable
$(TARGET): $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

# Rule to build object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean rule
clean:
	rm -f $(OBJS) $(TARGET)

# Phony targets
.PHONY: all clean
