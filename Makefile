# Makefile -- makefile(GNU) for a2r
CC	= gcc
CFLAGS	= -O -Wall
PREFIX	= /usr/local

.PHONY: clean
c2p:
	$(CC) $(CFLAGS) -o a2r a2r.c

clean:
	rm -f a2r

install:
	chmod a+x a2r
	cp a2r $(PREFIX)/bin/

uninstall:
	rm -f $(PREFIX)/bin/a2r

