# OS identification from: https://stackoverflow.com/questions/714100/os-detecting-makefile
OS := $(shell uname -s)

ifeq ($(OS), Darwin) 
  INCLUDE_PATH := /opt/homebrew/Cellar/criterion/2.4.1_1/include
  LIB_PATH := /opt/homebrew/Cellar/criterion/2.4.1_1/lib
endif
ifeq ($(OS), Linux) 
  INCLUDE_PATH := /util/criterion/include
  LIB_PATH := /util/criterion/lib/x86_64-linux-gnu
endif

CC := gcc
CFLAGS := -std=c11 -Wall

OBJECTS := csv.o tests.o

code.o: csv.c
	$(CC) $(CFLAGS) -c csv.c

tests.o: tests.c csv.h
	$(CC) $(CFLAGS) -I $(INCLUDE_PATH) -c tests.c

tests: tests.o csv.o
	$(CC) $(CFLAGS) -L $(LIB_PATH) -I $(INCLUDE_PATH) -o tests $(OBJECTS) -lcriterion

clean:
	rm -f *~ $(OBJECTS) tests