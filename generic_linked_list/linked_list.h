#pragma once
#include <stdlib.h>
#include <stdbool.h>

typedef struct linked_list {
  void *value;
  struct linked_list *next;
} linked_list;

linked_list *linked_list_init(void* value, linked_list *other);
void linked_list_add(linked_list *list, void *value);
bool linked_list_remove(linked_list *list,
			bool (*is_equal)(void *first,
					 void *second),
			void *value);
linked_list *linked_list_find(linked_list *list,
			      bool (*is_equal)(void *first,
					 void *second),
			      void *value);
void linked_list_print(linked_list *list, void (*print_value) (void *value));
void linked_list_free(linked_list *list);
