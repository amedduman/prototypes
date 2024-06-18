# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Iinclude

# Linker flags
LDFLAGS = -Llib -lraylib -framework CoreFoundation -framework IOKit -framework Cocoa

# Source files
SRCS = main.c $(wildcard src/*.c)

# Object files directory
OBJDIR = bin
OBJS = $(patsubst %.c,$(OBJDIR)/%.o,$(notdir $(SRCS)))

# Executable name
TARGET = game

# Default rule
all: $(TARGET)

# Rule to build the target executable
$(TARGET): $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

# Rule to build object files
$(OBJDIR)/%.o: %.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Create the object directory if it doesn't exist
$(OBJDIR):
	mkdir -p $(OBJDIR)

# Clean rule
clean:
	rm -f $(OBJS) $(TARGET)

# Phony targets
.PHONY: all clean
