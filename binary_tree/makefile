COMMON= -Wall -Wextra -Wfloat-equal -Wpedantic -Wvla -std=c99 -Werror
DEBUG= -g3
SANITIZE= $(COMMON) -fsanitize=undefined -fsanitize=address $(DEBUG)

all: binary_tree.h binary_tree.c
	clang binary_tree.c $(SANITIZE)

min: binary_tree.h binary_tree.c
	clang binary_tree.c

run:
	./a.out
