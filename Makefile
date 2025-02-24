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

CC = gcc
CFLAGS = -Wall -std=c11 -ggdb

# Object files
OBJS = csv.o runner.o

# Default target: build everything
all: csv tests

# Compile CSV program
csv: csv.o
	$(CC) $(CFLAGS) -o csv csv.o

# Compile runner
runner: csv.o runner.o
	$(CC) $(CFLAGS) -o runner $(OBJS)

# Compile CSV object file
csv.o: csv.c csv.h
	$(CC) -c $(CFLAGS) csv.c

# Compile runner object file
runner.o: runner.c csv.h
	$(CC) -c $(CFLAGS) runner.c

# Compile test object file
tests.o: tests.c csv.h
	$(CC) -c $(CFLAGS) -I $(INCLUDE_PATH) tests.c

# Compile tests
tests: csv.o tests.o
	$(CC) $(CFLAGS) -L $(LIB_PATH) -I $(INCLUDE_PATH) -o tests csv.o tests.o -lcriterion

# Clean up build files
.PHONY: clean
clean:
	rm -rf *~ *.o csv tests runner *.dSYM
