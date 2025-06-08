# Makefile for ncurses-based hex editor

CC = gcc
CFLAGS = -Wall -Wextra -g -O3
LDFLAGS = -lncurses
TARGET = hexagon
SRC = hexagon.c
TESTFILE = test.bin

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(SRC) $(lastword $(MAKEFILE_LIST))
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC) $(LDFLAGS)

# Declare that test.bin is a file we can build
$(TESTFILE):
	head -c 1024 /dev/urandom > $(TESTFILE)

run: $(TARGET) $(TESTFILE)
	./$(TARGET) $(TESTFILE)

clean:
	rm -f $(TARGET) $(TESTFILE)