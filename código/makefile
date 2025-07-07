CC = gcc
PKG_CFLAGS = $(shell pkg-config --cflags gtk4)
PKG_LIBS   = $(shell pkg-config --libs gtk4)
INCLUDES   = -Iinclude
SRCS       = src/main.c src/escalonador.c src/grafico.c
OBJS       = $(SRCS:.c=.o)
TARGET     = bin/app.exe

.PHONY: all build run clean

all: build

build: $(TARGET)

$(TARGET): $(OBJS)
	@mkdir -p $(dir $@)
	$(CC) -o $@ $^ $(PKG_LIBS)

%.o: %.c
	$(CC) $(INCLUDES) $(PKG_CFLAGS) -c $< -o $@

run: build
	./$(TARGET)

clean:
	rm -f src/*.o $(TARGET)
