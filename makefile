# Using µnit is very simple; just include the header and add the C
# file to your sources.  That said, here is a simple Makefile to build
# the example.

CSTD:=99
OPENMP:=n
ASAN:=n
UBSAN:=n
EXTENSION:=
TEST_ENV:=
CFLAGS:=-DMUNIT_NO_FORK
AGGRESSIVE_WARNINGS=n

ifeq ($(CC),pgcc)
        CFLAGS+=-c$(CSTD)
else
        CFLAGS+=-std=c$(CSTD)
endif

ifeq ($(OPENMP),y)
        ifeq ($(CC),pgcc)
                CFLAGS+=-mp
        else
                CFLAGS+=-fopenmp
        endif
endif

ifneq ($(SANITIZER),)
        CFLAGS+=-fsanitize=$(SANITIZER)
endif

ifneq ($(CC),pgcc)
        ifeq ($(EXTRA_WARNINGS),y)
                CFLAGS+=-Wall -Wextra -Werror
        endif

        ifeq ($(ASAN),y)
                CFLAGS+=-fsanitize=address
        endif

        ifeq ($(UBSAN),y)
                CFLAGS+=-fsanitize=undefined
        endif
endif

test_malloc$(EXTENSION): munit.h munit.c munit_my_malloc.c 
	$(CC) $(CFLAGS) -g -o $@ munit.c munit_my_malloc.c 

malloc$(EXTENSION): munit.h munit.c src/my_malloc.c munit_my_malloc.c 
	$(CC) $(CFLAGS) -g -o $@ munit.c src/my_malloc.c munit_my_malloc.c 

example$(EXTENSION): munit.h munit.c example.c
	$(CC) $(CFLAGS) -g -o $@ munit.c example.c

test:
	$(TEST_ENV) ./example$(EXTENSION)

clean:
	rm -f example$(EXTENSION)

all: example$(EXTENSION)