# Makefile -- makefile(GNU) for a2r
CC	= cc
CFLAGS	=  -std=c99 -O2 -Wall -Wextra -pipe
LDFLAGS = -lm
PREFIX	= /usr/local

SRCS = a2r.c
OBJS = $(SRCS:.c=.o)
PROGS = $(OBJS:.o=)

HAVE_LIBBSD = $(shell ld -lbsd && echo 1 || echo 0)
BSD_INCLUDES = $(word 2, $(shell whereis bsd))
UNAME = $(patsubst %BSD, BSD, $(shell uname))

ifeq ($(HAVE_LIBBSD), 1)
CFLAGS += -DHAVE_LIBBSD=1
LDFLAGS += -lbsd
endif

ifeq ($(UNAME), "BSD")
LDFLAGS += -lc
endif

.PHONY: clean tidy

all: $(PROGS)
	@$(RM) *.out
#	$(CC) $(CFLAGS) $(LDFLAGS) a2r.c -o a2r

echo:
	@echo "\$$(CFLAGS): $(CFLAGS)"
	@echo "\$$(HAVE_LIBBSD): $(HAVE_LIBBSD)"

debug:
	$(CC) $(CFLAGS) -g -ggdb $(LDFLAGS) -o  a2r a2r.c

tidy:
	$(RM) *.out || true
	$(RM) $(OBJS) || true

clean: tidy
	$(RM) $(PROGS) || true

install:
	chmod a+x a2r
	cp a2r $(PREFIX)/bin/

uninstall:
	rm -f $(PREFIX)/bin/a2r

static:
	$(CC) $(CFLAGS) $(LDFLAGS) --static -o a2r a2r.c

#.c.:
#	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $<
