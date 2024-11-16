CC := "/opt/homebrew/opt/llvm/bin/clang"
COMMON= -Wall -Wextra -Wfloat-equal -Wpedantic -Wvla -std=c99 -Werror
DEBUG= -g3
SANITIZE= $(COMMON) -fsanitize=undefined -fsanitize=address $(DEBUG)

run: stack.h stack.c
	clang stack.c $(SANITIZE)
