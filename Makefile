CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11
SOURCES = graph.c token.c per_rpn.c calc.c
BUILD_DIR = ../build
TARGET = $(BUILD_DIR)/graph

$(TARGET): $(SOURCES)
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCES) -lm

all: $(TARGET)

clean:
	rm -rf $(TARGET) $(BUILD_DIR)

.PHONY: clean