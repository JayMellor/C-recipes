#ifndef LINKED_LIST

#  define LINKED_LIST

typedef struct Cons {
  char car;
  struct Cons *cdr;
} Cons;

Cons *cons(char car, Cons *cdr);
Cons *copy_cons(Cons *existing_cons);
Cons *last(Cons* cons);
Cons *append(Cons *first, Cons *second);
bool insert_after(Cons* current, char new_value);
void print_cons(Cons *cons);

#endif
