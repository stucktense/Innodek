CC ?= gcc
CFLAGS ?= -Wall -Wextra -std=c99 -Iinclude

SRC = src/main.c src/value.c src/env.c src/parser.c
OBJ = $(SRC:.c=.o)
TARGET = lang

PREFIX ?= /usr/local
BINDIR = $(PREFIX)/bin
LIBDIR = $(PREFIX)/lib/lang

.PHONY: all install clean

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

install: $(TARGET)
	mkdir -p $(BINDIR)
	mkdir -p $(LIBDIR)
	cp $(TARGET) $(BINDIR)/lang
	chmod 755 $(BINDIR)/lang
	@echo "Successfully installed lang to $(BINDIR)/lang"

clean:
	rm -f src/*.o $(TARGET)
