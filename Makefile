CC      := gcc
GIT_REV := $(shell git rev-parse --short HEAD 2>/dev/null || echo unknown)
CFLAGS  := -std=c11 -Wall -Wextra -Werror -O2 -Iinclude -DGIT_HASH=\"$(GIT_REV)\"
ASAN    := -std=c11 -Wall -Wextra -Werror -g -O1 -fsanitize=address,undefined -Iinclude -DGIT_HASH=\"$(GIT_REV)\"
LDFLAGS := -lm

LIB_SRC := $(wildcard src/*.c)
LIB_OBJ := $(LIB_SRC:.c=.o)

all: tokamak_cli

tokamak_cli: $(LIB_OBJ) apps/tokamak_cli.c
	$(CC) $(CFLAGS) -o $@ $(LIB_OBJ) apps/tokamak_cli.c $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

test: $(LIB_OBJ) tests/test_s1.c
	$(CC) $(CFLAGS) -o test_s1 $(LIB_OBJ) tests/test_s1.c $(LDFLAGS)
	./test_s1

asan:
	$(CC) $(ASAN) -o test_s1_asan $(LIB_SRC) tests/test_s1.c $(LDFLAGS)
	./test_s1_asan

figures: tokamak_cli
	./tokamak_cli
	python3 scripts/plot_mask.py

clean:
	rm -f $(LIB_OBJ) tokamak_cli test_s1 test_s1_asan
