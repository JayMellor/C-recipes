#include <assert.h>
#include "word_list_linked.h"

WordList *word_list_init() {
  WordList *word_list = (WordList *) malloc(sizeof(WordList));
  word_list->length = 0;
  return word_list;
}

char *alloc_value(char *value) {
  char *new_value = (char *) malloc(strlen(value) + 1);
  strcpy(new_value, value);
  return new_value;
}

void add_first_node(WordList *word_list, char *line) {
  word_list->first = linked_list_init(alloc_value(line), NULL);
  word_list->last = word_list->first;
  word_list->length++;
}

void add_node_after(WordList *word_list, linked_list *node, char *line) {
  char *alloc_line = alloc_value(line);
  if (node == NULL) {
    linked_list *list = linked_list_init(alloc_line, word_list->first);
    word_list->first = list;
  } else {
    linked_list_add(node, alloc_line);
    if (node->next->next == NULL) {
      word_list->last = node->next;
    }
  }
  word_list->length++;
}

void word_list_add(WordList *word_list, char *line) {
  if (word_list == NULL) {
    return;
  }
  if (word_list->first == NULL) {
    add_first_node(word_list, line);
    return;
  }
  if(strcmp(word_list->last->value, line) <= 0) {
    add_node_after(word_list, word_list->last, line);
    return;
  }

  linked_list *node = word_list->first;
  linked_list *prev = NULL;
  do {
    if (strcmp(node->value, line) >= 0) {
      add_node_after(word_list, prev, line);
      return;
    }
    prev = node;
    node = node->next;
  } while(node != NULL);
  add_node_after(word_list, prev, line);  
}

int word_list_size(WordList *word_list) {
  return word_list->length;
}

bool str_equal(void *first, void *second) {
  return strcmp((char *) first, (char *) second) == 0;
}

char *word_list_find(WordList *word_list, char *word) {
  linked_list *matching_node = linked_list_find(word_list->first, &str_equal, word);
  if (matching_node == NULL) {
    return NULL;
  } else {
    return matching_node->value;
  }
}

void print_str(void *line) {
  printf("%s", (char *) line);
}

void word_list_print(WordList *word_list) {
  linked_list_print(word_list->first, &print_str);
}

void free_node(linked_list *list) {
  if (list == NULL) {
    return;
  }
  linked_list *next = list->next;
  linked_list_free(list);
  return free_node(next);
}

void word_list_free(WordList *word_list) {
  free_node(word_list->first);
  free(word_list);
}



bool word_list_is_sorted(WordList *word_list) {
  if (word_list == NULL) {
    return false;
  }
  if (word_list->first == NULL) {
    return false;
  }
  linked_list *list = word_list->first;
  while (list->next != NULL) {
    if (strcmp(list->value, list->next->value) > 0) {
      printf("first mismatch is %s > %s\n", (char *) list->value, (char *) list->next->value);
      return false;
    }
    list = list->next;
  }
  return true;
}

void test() {
  WordList *word_list = word_list_init();
  assert(word_list->length == 0);
  assert(word_list->first == NULL);
  assert(word_list->last == NULL);

  word_list_add(word_list, "first line");
  assert(word_list->length == 1);
  assert(strcmp(word_list->first->value, "first line") == 0);

  word_list_add(word_list, "alphabet");
  assert(word_list->length == 2);
  assert(strcmp(word_list->first->value, "alphabet") == 0);
  assert(strcmp(word_list->first->next->value, "first line") == 0);

  word_list_add(word_list, "gerald");
  word_list_add(word_list, "duren");
  word_list_add(word_list, "elephant");
  word_list_add(word_list, "ale");
  word_list_add(word_list, "zoologist");
  word_list_add(word_list, "xanthous");
  assert(word_list_is_sorted(word_list));

  assert(word_list_find(word_list, "first line") != NULL);
  assert(word_list_find(word_list, "not here") == NULL);

  word_list_free(word_list);

  printf("Tests succeeded\n");
}
