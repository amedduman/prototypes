# Compiler
CC = clang

# Directories
INCLUDE_DIR = include
LIB_DIR = lib
SRC_DIR = src

# Files
LIBS = $(LIB_DIR)/libraylib.a
FRAMEWORKS = -framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework OpenGL
INCLUDES = -I$(INCLUDE_DIR)
SRCS = $(wildcard $(SRC_DIR)/*.c) main.c
OBJS = $(SRCS:.c=.o)

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

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean rules
clean:
	rm -f $(OBJS) $(TARGET)

# Dependencies
-include $(OBJS:.o=.d)

%.d: %.c
	@$(CC) $(CFLAGS) -MM $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

.PHONY: all clean
