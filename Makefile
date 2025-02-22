CC = gcc
CFLAGS = -Wall -Wextra -std=c11
TARGET = csv
SRCS = csv.c
OBJS = $(SRCS:.c=.o)

# Criterion Test Variables (Platform-Specific)
ifeq ($(OS), Darwin)  # macOS
  INCLUDE_PATH := /opt/homebrew/Cellar/criterion/2.4.1_1/include
  LIB_PATH := /opt/homebrew/Cellar/criterion/2.4.1_1/lib
endif

ifeq ($(OS), Linux)
  INCLUDE_PATH := /util/criterion/include
  LIB_PATH := /util/criterion/lib/x86_64-linux-gnu
endif

TEST_TARGET = test_csv
TEST_SRCS = tests.c
TEST_OBJS = $(TEST_SRCS:.c=.o)
CRITERION_FLAGS = -I$(INCLUDE_PATH) -L$(LIB_PATH) -lcriterion

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJS) $(TEST_TARGET) $(TEST_OBJS)

# Criterion Tests
tests: $(TEST_TARGET)
	./$(TEST_TARGET)

$(TEST_TARGET): $(TEST_SRCS) $(OBJS)
	$(CC) $(CFLAGS) -o $(TEST_TARGET) $(TEST_SRCS) $(OBJS) $(CRITERION_FLAGS)

.PHONY: all clean tests
