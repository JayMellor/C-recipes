#include "linked_list.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>

char *make_string(char *value) {
  char *new_value = (char *) malloc(sizeof(*value));
  if (new_value == NULL) {
    exit(1);
  }
  
  strcpy(new_value, value);
  return new_value;
}

linked_list *make_list(char *value, linked_list *next) {
  char *new_value = make_string(value);
  return linked_list_init(new_value, next);
}

void print_str(void *str) {
  printf("%s", (char *) str);
}

void str_linked_list_print(linked_list *list) {
  linked_list_print(list, &print_str);
}

void str_linked_list_add(linked_list *list, char *value) {
  char *new_value = make_string(value);
  linked_list_add(list, new_value);
}

bool str_equal(void *first, void *second) {
  return strcmp((char *) first, (char *) second) == 0;
}

bool str_linked_list_remove(linked_list *list, char *value) {
  return linked_list_remove(list, &str_equal, value);
}

void test(void) {
  linked_list *first = make_list("The first node", NULL);
  assert(first != NULL);
  assert(strcmp(first->value, "The first node")== 0);
  
  linked_list *list = make_list("start",
				make_list("el 1",
					  make_list("el 2",
						    first)));
  assert(list != NULL);
  assert(strcmp(list->value, "start") == 0);
  assert(list->next != NULL);
  assert(strcmp(list->next->value, "el 1") == 0);
  assert(list->next->next != NULL);
  assert(strcmp(list->next->next->value, "el 2") == 0);
  assert(list->next->next->next == first);

  str_linked_list_add(list, "NEW VAL");
  assert(strcmp(list->next->value, "NEW VAL") == 0);
  assert(strcmp(list->next->next->value, "el 1") == 0);

  assert(str_linked_list_remove(list, "el 1"));
  assert(list != NULL);
  assert(str_equal(list->value, "start"));
  assert(str_equal(list->next->value, "NEW VAL"));
  assert(str_equal(list->next->next->value, "el 2"));
  assert(str_equal(list->next->next->next->value, "The first node"));

  linked_list_free(list);
}

int main() {
  test();
  return 0;
}
