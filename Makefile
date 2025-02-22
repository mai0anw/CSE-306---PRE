CC = gcc
CFLAGS = -Wall -Wextra -std=c11
TARGET = csv
SRCS = csv.c
OBJS = $(SRCS:.c=.o)

# Criterion Test Variables
TEST_TARGET = test_csv
TEST_SRCS = test_csv.c
TEST_OBJS = $(TEST_SRCS:.c=.o)
CRITERION_FLAGS = `pkg-config --cflags --libs criterion`

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