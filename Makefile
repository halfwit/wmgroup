include config.mk

HDR = util.h
SRC =           \
	grp.c \
	lsgrp.c

OBJ = $(SRC:.c=.o)
BIN = $(SRC:.c=)

.POSIX:

all: binutils

binutils: $(BIN)

$(OBJ): $(HDR) util.o

.o:
	@echo "LD $@"
	@$(LD) $< -o $@ $(LDFLAGS) util.o

.c.o:
	@echo "CC $<"
	@$(CC) -c $< -o $@ $(CFLAGS)

install: $(BIN)
	mkdir -p $(DESTDIR)$(PREFIX)/bin/
	cp -f $(BIN) $(DESTDIR)$(PREFIX)/bin/

uninstall:
	@echo "uninstalling binaries"
	@for util in $(BIN); do \
		rm -f $(DESTDIR)$(PREFIX)/bin/$$util; \
	done

clean:
	rm -f $(OBJ) $(BIN) util.o
