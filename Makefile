CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -Iinclude

SRC = src/main.c src/value.c src/env.c src/parser.c
OBJ = $(SRC:.c=.o)
TARGET = lang

PREFIX ?= /usr/local/bin
LIBDIR ?= /usr/local/lib/lang

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Install binary globally and create standard library directory
install: $(TARGET)
	mkdir -p $(PREFIX)
	mkdir -p $(LIBDIR)
	cp $(TARGET) $(PREFIX)/lang
	
	# Copy any standard library files into the global system path
	cp libs/*.fn $(LIBDIR)/ 2>/dev/null || true
	@echo "Successfully installed 'lang' and libraries to system paths."

uninstall:
	rm -f $(PREFIX)/lang
	rm -rf $(LIBDIR)
	@echo "Successfully uninstalled 'lang'."

clean:
	rm -f src/*.o $(TARGET)

.PHONY: all install uninstall clean
