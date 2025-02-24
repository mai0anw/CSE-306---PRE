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
OBJS = csv_functions.o csv.o

# Default target: build everything
all: csv_functions tests

# Compile csv_functions program
csv_functions: csv_functions.o
	$(CC) $(CFLAGS) -o csv_functions csv_functions.o

# Compile csv
csv: csv_functions.o csv.o
	$(CC) $(CFLAGS) -o csv $(OBJS)

# Compile csv_functions object file
csv_functions.o: csv_functions.c csv_functions.h
	$(CC) -c $(CFLAGS) csv_functions.c

# Compile csv object file
csv.o: csv.c csv_functions.h
	$(CC) -c $(CFLAGS) csv.c

# Compile test object file
tests.o: tests.c csv_functions.h
	$(CC) -c $(CFLAGS) -I $(INCLUDE_PATH) tests.c

# Compile tests
tests: csv_functions.o tests.o
	$(CC) $(CFLAGS) -L $(LIB_PATH) -I $(INCLUDE_PATH) -o tests csv_functions.o tests.o -lcriterion

# Clean up build files
.PHONY: clean
clean:
	rm -rf *~ *.o csv_functions tests csv *.dSYM
