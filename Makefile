CC = gcc
CFLAGS = -Wall -I./include
LDFLAGS = $(shell pkg-config --cflags --libs gtk+-3.0)

TARGET = Pong

SRCS = src/main.c
OBJS = $(SRCS:src/%.c=build/%.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(OBJS) $(LDFLAGS)

build/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

