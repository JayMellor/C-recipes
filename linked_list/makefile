CC := "/opt/homebrew/opt/llvm/bin/clang"
COMMON= -Wall -Wextra -Wfloat-equal -Wpedantic -Wvla -std=c99 -Werror
DEBUG= -g3
SANITIZE= $(COMMON) -fsanitize=undefined -fsanitize=address $(DEBUG)

run: linked_list.c linked_list.h
	clang linked_list.c $(SANITIZE)
