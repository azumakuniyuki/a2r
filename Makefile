# Makefile -- makefile(GNU) for a2r
CC	= gcc
CFLAGS	= -O2 -Wall -Wextra
LDFLAGS = -lm
PREFIX	= /usr/local

SOURCES = a2r.c
OBJECTS = $(SOURCES:.c=.o)
PROGRAMS = $(OBJECTS:.o=)

HAVE_LIBBSD = $(shell ld -lbsd && echo 1 || echo 0)
BSD_INCLUDES = $(word 2, $(shell whereis bsd))

ifeq ($(HAVE_LIBBSD), 1)
CFLAGS += -DHAVE_LIBBSD=1
LDFLAGS += -lbsd
endif

.PHONY: clean
all: $(OBJECTS) $(PROGRAMS)
#	$(CC) $(CFLAGS) $(LDFLAGS) -o a2r a2r.c

echo:
	@echo "\$$(CFLAGS): $(CFLAGS)"
	@echo "\$$(HAVE_LIBBSD): $(HAVE_LIBBSD)"

debug:
	$(CC) $(CFLAGS) -g -ggdb $(LDFLAGS) -o a2r a2r.c

clean:
	$(RM) $(PROGRAMS) || true
	$(RM) $(OBJECTS) || true
	$(RM) *.out || true

install:
	chmod a+x a2r
	cp a2r $(PREFIX)/bin/

uninstall:
	rm -f $(PREFIX)/bin/a2r

static:
	$(CC) $(CFLAGS) $(LDFLAGS) --static -o a2r a2r.c

