#pragma once

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

typedef struct Cons {
  struct Cons *car;
  struct Cons *cdr;
  void *atom;
  size_t size;
} Cons;

Cons *new_cons(void);
Cons *atom(void *value, size_t size);
Cons *cons(Cons *car, Cons *cdr);
bool is_atomic(Cons *cons);
void print_cons(Cons *cons);
void delete_cons(Cons *cons);

/**
 * Input parsing
 * - (CONS 1 2) => (1 . 2)
 * - (CONS 1 (CONS 2 3)) => (1 . (2 . 3))
 * - (ATOM 1) => NULL
 * - (CONS 1 2) => {CONS 1 2}
 * - (CONS 1 (CONS 2 3)) -> {CONS 1 {CONS 2 3}}
 */

Cons *parse(char *input);
