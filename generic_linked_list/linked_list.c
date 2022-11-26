#include <stdio.h>
#include "linked_list.h"

linked_list *linked_list_init(void *value, linked_list *next) {
  if (value == NULL) {
    return NULL;
  }
  
  linked_list *list = (linked_list *) malloc(sizeof(linked_list));
  list->value = value;
  list->next = next;
  return list;
}

void linked_list_add(linked_list *list, void *value) {
  if (list == NULL || value == NULL) {
    // Nothing to add or add to
    return;
  }

  linked_list *new_list = linked_list_init(value, list->next);
  list->next = new_list;
}


bool linked_list_remove(linked_list *list,
			bool (*is_equal)(void *first,
					 void *second),
			void *value) {
  if (list == NULL || value == NULL) {
    return false;
  }

  linked_list *prev = NULL;
  do {
    if (is_equal(list->value, value)) {
      if (prev != NULL) {
	prev->next = list->next;
      }
      free(list);
      return true;
    }
    prev = list;
    list = list->next;
  } while (list != NULL);

  return false;
}

linked_list *linked_list_find(linked_list *list,
			      bool (*is_equal)(void *first,
					       void *second),
			      void *value) {
  if (list == NULL || is_equal == NULL || value == NULL) {
    return NULL;
  }

  do {
    if (is_equal(list->value, value)) {
      return list;
    }
    list = list->next;
  } while (list != NULL);

  return NULL;
}

void linked_list_print(linked_list *list, void (*print_value) (void *value)) {
  if (list == NULL || print_value == NULL) {
    return;
  }

  do {
    print_value(list->value);
    list = list->next;
    if (list != NULL) {
      printf("->");
    }
  } while (list != NULL);
  printf("\n");  
}

void linked_list_free(linked_list *list) {
  free(list->value);
  free(list);
}
