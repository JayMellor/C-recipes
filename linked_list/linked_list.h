#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

typedef struct Cons {
  struct Cons *car; // can be atomic
  struct Cons *cdr; // can be atomic
  void *atom; // atoms are copied
} Cons;

Cons *new_cons(void);
Cons *atom(void *value, size_t size);
bool is_atomic(Cons *cons);
void *copy_value(void *value, size_t size);
Cons *cons(Cons *car, Cons *cdr);
Cons *copy_list(Cons *existing_cons);
Cons *last(Cons* cons);
Cons *append(Cons *first, Cons *second);
bool insert_after(Cons* current, Cons *new);
Cons *mapcar(void (*map_value) (void *car), Cons *cons);
void list_to_string(char *list_string,
		    Cons *cons,
		    void (*to_string) (char* list_string, void *atom));
void free_list(Cons *cons);
