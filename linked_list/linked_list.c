#include "linked_list.h"

// Moving towards Conses with possible void *atoms

void string_to_string(char *list_string, void *atom) {
  if (atom == NULL) {
    return;
  }
  strcat(list_string, atom);
}

void int_to_string(char *list_string, void *atom) {
  if (atom == NULL) {
    return;
  }
  
  snprintf(list_string, sizeof(list_string), "%s%i", list_string, *((int *) atom));
}

void test(void) {
  Cons *single = cons(atom("fred", sizeof("fred")), NULL);
  assert(strcmp(single->car->atom, "fred") == 0);
  assert(single->cdr == NULL);
  int one = 1;
  int two = 2;
  Cons *another = cons(cons(atom(&one, sizeof(int)),
			    cons(atom(&two, sizeof(int)), NULL)),
		       NULL);
  assert(another->car != NULL);
  assert(another->car->car != NULL);
  assert(*((int *) another->car->car->atom) == 1);
  assert(*((int *) another->car->cdr->car->atom) == 2);
  assert(another->cdr == NULL);

  char str[100] = "";
  list_to_string(str, single, &string_to_string);
  puts(str);
  strcpy(str, "");
  list_to_string(str, another, &int_to_string);
  printf("%s\n", str);

  free_list(single);
  free_list(another);

  /* Cons *pair = cons('B', single); */
  /* assert(pair->car == 'B'); */
  /* assert(pair->cdr == single); */

  /* Cons *copy = copy_cons(pair); */
  /* assert(copy != pair); */
  /* assert(copy->car == 'B'); */
  /* assert(copy->cdr != single); */
  /* assert(copy->cdr->car == 'A'); */
  /* assert(copy->cdr->cdr == NULL); */

  /* Cons *joined = append(single, pair); */
  /* assert(joined->car == 'A'); */
  /* assert(joined->cdr->car == 'B'); */
  /* assert(joined->cdr->cdr->car == 'A'); */
  /* assert(joined->cdr->cdr->cdr == NULL); */

  /* Cons *my_list = cons('A', cons('Z', cons('Q', cons('X', NULL)))); */
  /* Cons *q_cons = my_list->cdr->cdr; */
  /* insert_after(q_cons, 'Y'); */
  /* assert(q_cons->cdr->car == 'Y'); */
  /* assert(q_cons->cdr->cdr->car == 'X'); */

  /* Cons *mapped_cons = mapcar(&dec, my_list); */
  /* print_cons(mapped_cons); */
  /* assert(mapped_cons->car == '@'); */
  /* assert(mapped_cons->cdr != NULL); */
  /* assert(mapped_cons->cdr->car == 'Y'); */
  /* assert(my_list->car == 'A'); */
  /* printf("%s", typeof(mapped_cons)); */

  /* free_list(my_list); */
  /* free_list(copy); */
  /* free_list(joined); */
  /* free_list(mapped_cons); */
}

int main(void) {
  test();
  return 0;
}

Cons *new_cons(void) {
  Cons *cons = (Cons *) malloc(sizeof(Cons));
  if (cons == NULL) {
    exit(EXIT_FAILURE);
  }
  return cons;
}

void *copy_value(void *value, size_t size) {
  void *copy = malloc(size);
  if (copy == NULL) {
    exit(EXIT_FAILURE);
  }
  memcpy(copy, value, size);
  return copy;
}

Cons *atom(void *value, size_t size) {
  Cons *cons = new_cons();
  cons->car = NULL;
  cons->cdr = NULL;
  cons->atom = copy_value(value, size);
  return cons;
}

bool is_atomic(Cons *cons) {
  return (cons != NULL && cons->atom != NULL);
}

Cons *cons(Cons *car, Cons *cdr) {
  Cons *cons = new_cons();
  cons->car = car;
  cons->cdr = cdr;
  cons->atom = NULL;
  return cons;
}

Cons *copy_cons(Cons *existing) {
  if (existing == NULL) {
    return NULL;
  }
  return cons(existing->car, copy_cons(existing->cdr));
}

Cons *last(Cons* cons) {
    for (; cons != NULL; cons = cons->cdr) {
      if (cons->cdr == NULL) {
	return cons;
      }
    }
    return NULL;
}

Cons *append(Cons *first, Cons *second) {
  Cons *first_copy = copy_cons(first);
  Cons *last_cons = last(first_copy);
  if (last_cons == NULL) {
    return second;
  }
  last_cons->cdr = second;
  return first_copy;
}

bool insert_after(Cons* current, Cons *new) {
  if (current == NULL) {
    return false;
  }
  if (new == NULL) {
    return false;
  }
  if (is_atomic(current) || is_atomic(new)) {
    return false;
  }
  Cons *new_cons = cons(new, current->cdr);
  current->cdr = new_cons;
  return true;
}


// Need to think about cars being Cons or atoms
/* Cons *mapcar(void (*map_value) (void *value), Cons *cons) { */
/*   if (cons == NULL) { */
/*     return NULL; */
/*   } */
/*   if (map_value == NULL) { */
/*     return NULL; */
/*   } */

/*   Cons *new_cons = (Cons *) malloc(sizeof(Cons)); */
/*   new_cons->car = map_value(cons->car); */
/*   new_cons->cdr = mapcar(map_value, cons->cdr); */
/*   return new_cons; */
/* } */

void list_to_string(char *list_string,
		    Cons *cons,
		    void (*to_string) (char* list_string, void *atom)) {
  if (cons == NULL) {
    return;
  }
  if (is_atomic(cons)) {
    to_string(list_string, cons->atom);
    return;
  }
  strcat(list_string, "(");
  list_to_string(list_string, cons->car, to_string);
  list_to_string(list_string, cons->cdr, to_string);
  strcat(list_string, ")");
}

void free_list(Cons *cons) {
  if (cons == NULL) {
    return;
  }
  if (is_atomic(cons)) {
    free(cons->atom);
  } else {
    free_list(cons->car);
    free_list(cons->cdr);    
  }
  free(cons);
}

char dec(char ch) {
  return ch - 1;
}
