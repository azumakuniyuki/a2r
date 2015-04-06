# Makefile -- makefile(GNU) for a2r
CC	= gcc
CFLAGS	= -O -Wall
LDFLAGS = -lm
PREFIX	= /usr/local

HAVE_LIBBSD = $(shell ld -lbsd && echo 1 || echo 0)
BSD_INCLUDES = $(word 2, $(shell whereis bsd))

ifeq ($(HAVE_LIBBSD), 1)
CFLAGS += -DHAVE_LIBBSD=1
LDFLAGS += -lbsd
endif

all:
	@echo "\$$(CFLAGS): $(CFLAGS)"
	@echo "\$$(HAVE_LIBBSD): $(HAVE_LIBBSD)"

.PHONY: clean
c2p:
	$(CC) $(CFLAGS) $(LDFLAGS) -o a2r a2r.c

debug:
	$(CC) $(CFLAGS) -g -ggdb -Wextra $(LDFLAGS) -o a2r a2r.c

clean:
	rm -f a2r

install:
	chmod a+x a2r
	cp a2r $(PREFIX)/bin/

uninstall:
	rm -f $(PREFIX)/bin/a2r

