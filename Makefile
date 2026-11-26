CC     = gcc
CFLAGS = -std=c11 -Wall -Wextra -Werror -O2 -g -Iinclude/src
ASAN_FLAGS = -fsanitize=address,undefined -g
LDLIBS = -lm
SRC = include/src/params.c

OBJ = $(SRC:.c=.o)

.PHONY : all test asan figures clean 

all: tokamak_cli

tokamak_cli: apps/tokamak_cli.c $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

test: 
	$(CC) $(CFLAGS) tests/test_main.c $(OBJ) -o test_runner && ./test_runner

asan: CFLAGS += $(ASAN_FLAGS)
asan: clean all 

figures:
		python3 scripts/plot_results.py

clean:
	rm -f $(OBJ) tokamak_cli test_runner

