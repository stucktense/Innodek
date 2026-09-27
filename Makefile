CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -Iinclude

SRC = src/main.c src/value.c src/env.c src/parser.c
OBJ = $(SRC:.c=.o)
TARGET = lang

# Automatically detects Termux's $PREFIX environment variable
PREFIX ?= /usr/local
BINDIR = $(PREFIX)/bin
LIBDIR = $(PREFIX)/lib/lang

DEB_DIR = package
BIN_DEST = $(DEB_DIR)/usr/local/bin

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

install: $(TARGET)
	mkdir -p $(BINDIR)
	mkdir -p $(LIBDIR)
	cp $(TARGET) $(BINDIR)/lang
	cp libs/*.fn $(LIBDIR)/ 2>/dev/null || true
	@echo "Successfully installed 'lang' to $(BINDIR)/lang"

uninstall:
	rm -f $(BINDIR)/lang
	rm -rf $(LIBDIR)
	@echo "Successfully uninstalled 'lang'"
deb: $(TARGET)
	mkdir -p $(BIN_DEST)
	cp $(TARGET) $(BIN_DEST)/lang
	chmod 755 $(BIN_DEST)/lang
	# Set required permissions for Debian maintainer scripts
	chmod 755 $(DEB_DIR)/DEBIAN/postinst
	chmod 755 $(DEB_DIR)/DEBIAN/prerm
	dpkg-deb --build $(DEB_DIR) innodek_1.0.0_amd64.deb
	@echo "Created innodek_1.0.0_amd64.deb successfully!"

clean:
	rm -f src/*.o $(TARGET)

.PHONY: all install uninstall clean

