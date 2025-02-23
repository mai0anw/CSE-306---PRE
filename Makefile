# OS identification
OS := $(shell uname -s)

ifeq ($(OS), Darwin)
  INCLUDE_PATH := /opt/homebrew/Cellar/criterion/2.4.1_1/include
  LIB_PATH := /opt/homebrew/Cellar/criterion/2.4.1_1/lib
endif
ifeq ($(OS), Linux)
  INCLUDE_PATH := /util/criterion/include
  LIB_PATH := /util/criterion/lib/x86_64-linux-gnu
endif

# Compiler and flags
CC := gcc
CFLAGS := -std=c11 -Wall

# Object files
OBJECTS := csv.o tests.o

# Default target: Build the tests
all: tests

# Compile csv.o from csv.c
csv.o: csv.c csv.h
    $(CC) $(CFLAGS) -c $< -o $@

# Compile tests.o from test_csv.c
tests.o: test_csv.c csv.h
    $(CC) $(CFLAGS) -I $(INCLUDE_PATH) -c $< -o $@

# Link object files and build the tests executable
tests: $(OBJECTS)
    $(CC) $(CFLAGS) -L $(LIB_PATH) -I $(INCLUDE_PATH) -o tests $(OBJECTS) -lcriterion

# Clean build artifacts
clean:
    rm -f *~ $(OBJECTS) tests
