CC = gcc
CFLAGS := -Wall -I./include $(shell pkg-config --cflags gtk+-3.0)
LDFLAGS := -lm $(shell pkg-config --libs gtk+-3.0)

TARGET = Pong

SRCS = src/pong.c
OBJS = $(SRCS:src/%.c=build/%.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(OBJS) $(LDFLAGS)

build/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf build Pong
	mkdir build

