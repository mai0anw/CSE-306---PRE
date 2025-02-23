# Compiler and flags
CC := gcc
CFLAGS := -std=c11 -Wall -Wextra -pedantic

# OS-specific paths for Criterion (testing framework)
OS := $(shell uname -s)

ifeq ($(OS), Darwin) 
  INCLUDE_PATH := /opt/homebrew/Cellar/criterion/2.4.1_1/include
  LIB_PATH := /opt/homebrew/Cellar/criterion/2.4.1_1/lib
endif
ifeq ($(OS), Linux) 
  INCLUDE_PATH := /util/criterion/include
  LIB_PATH := /util/criterion/lib/x86_64-linux-gnu
endif

# Target executables
TARGET := csv
TEST_TARGET := tests

# Source files
SRCS := csv.c
TEST_SRCS := tests.c

# Object files
OBJS := $(SRCS:.c=.o)
TEST_OBJS := $(TEST_SRCS:.c=.o)

# Build the main CSV processing executable
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Build tests with Criterion
tests: $(TEST_OBJS) $(OBJS)
	$(CC) $(CFLAGS) -I$(INCLUDE_PATH) -L$(LIB_PATH) -o $(TEST_TARGET) $(TEST_OBJS) $(OBJS) -lcriterion

# Compile source files into object files
%.o: %.c
	$(CC) $(CFLAGS) -I$(INCLUDE_PATH) -c $< -o $@

# Clean build artifacts
clean:
	rm -f $(TARGET) $(TEST_TARGET) $(OBJS) $(TEST_OBJS)
