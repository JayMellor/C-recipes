#include "word_list_array.h"
#include <assert.h>

WordList *word_list_init() {
  WordList *word_list = (WordList *) malloc(sizeof(WordList));
  word_list->el = (char **) malloc(CHUNK_SIZE * sizeof(char *));
  word_list->length = 0;
  return word_list;
}

// a 0
// b 1
// <-
// d 2
// f 3
//   4

// c 2
// length == 4

// Finds the index LINE should have when part of WORD_LIST when sorting alphabetically
int find_place(WordList *word_list, char *line) {
  if (word_list->length == 0) {
    return 0;
  }
  if (strcmp(line, word_list->el[0]) <= 0) {
    return 0;
  }
  for (int index = 0; index < word_list->length - 1; index++) {
    if (strcmp(word_list->el[index], line) <= 0 &&
	strcmp(word_list->el[index + 1], line) >=0) {
      return index + 1;
    }
  }
  return word_list->length;
}

void expand_list(WordList *word_list) {
  if (++word_list->length % CHUNK_SIZE == 0) {
    word_list->el = (char **) realloc(word_list->el,
				      (word_list->length + CHUNK_SIZE) * sizeof(char *));
  }
  word_list->el[word_list->length - 1] = (char *) malloc(LINE_LENGTH + 1);
}

void word_list_add(WordList *word_list, char *line) {
  int position = find_place(word_list, line);
  expand_list(word_list);
  for (int later_index = word_list->length - 1; later_index > position; later_index--) {
    strcpy(word_list->el[later_index], word_list->el[later_index - 1]);
  }
  strcpy(word_list->el[position], line);
}

int word_list_size(WordList *word_list) {
  return word_list->length;
}

// Searching is super fast
char *word_list_find(WordList *word_list, char *word) {
  for (int position = 0; position < word_list->length; position++) {
    if (strcmp(word_list->el[position], word) == 0) {
      return word_list->el[position];
    }
  }
  return NULL;
}

void word_list_print(WordList *word_list) {
  for (int position = 0; position < word_list->length; position++) {
    printf("%i. %s", position, word_list->el[position]);
  }
  printf("\n");
}

void word_list_free(WordList *word_list) {
  for (int position = 0; position < word_list->length; position++) {
    free(word_list->el[position]);
  }
  free(word_list->el);
  free(word_list);
}

void test() {
  WordList *word_list = word_list_init();
  assert(word_list != NULL);
  assert(word_list->length == 0);

  word_list_add(word_list, "first word");
  assert(word_list->length == 1);
  assert(strcmp(word_list->el[0], "first word") == 0);

  word_list_add(word_list, "alphabet");
  assert(word_list->length == 2);
  assert(strcmp(word_list->el[0], "alphabet") == 0);
  assert(strcmp(word_list->el[1], "first word") == 0);

  assert(word_list_find(word_list, "first word") != NULL);
  assert(word_list_find(word_list, "not here") == NULL);

  word_list_free(word_list);

  printf("Tests succeeded\n");
}
