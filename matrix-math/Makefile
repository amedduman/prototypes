# Compiler
CC = gcc

# Directories
INCLUDE_DIR = include
LIB_DIR = lib
SRC_DIR = src
BUILD_DIR = build

# Files
LIBS = $(LIB_DIR)/libraylib.a
FRAMEWORKS = -framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework OpenGL
INCLUDES = -I$(INCLUDE_DIR)
SRCS = $(wildcard $(SRC_DIR)/*.c) main.c
OBJS = $(patsubst %.c, $(BUILD_DIR)/%.o, $(SRCS))
DEPS = $(OBJS:.o=.d)

# Output
TARGET = game

# Compiler flags
CFLAGS = -Wall -Wextra -Werror $(INCLUDES)

# Linker flags
LDFLAGS = $(LIBS) $(FRAMEWORKS)

# Build rules
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

$(BUILD_DIR)/%.o: %.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Clean rules
clean:
	rm -rf $(BUILD_DIR) $(TARGET)

# Dependencies
-include $(DEPS)

$(BUILD_DIR)/%.d: %.c | $(BUILD_DIR)
	@$(CC) $(CFLAGS) -MM $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,$(BUILD_DIR)/\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

.PHONY: all clean
