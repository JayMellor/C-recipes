#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../linked_list/linked_list.h"
#define FILENAME "./eng_370k_shuffle.txt"
#define LINE_LENGTH 100
#define FILE_LENGTH 370000

typedef struct WordList {
  char **el;
  int length;
} WordList;

int min(int first, int second) {
  return first < second ? first : second;
}

int max(int first, int second) {
  return first > second ? first : second;
}

void print_list(WordList *word_list) {
  for (int position = 0; position < word_list->length; position++) {
    printf("%i. %s", position, word_list->el[position]);
  }
  printf("\n");
}

bool sort_alpha(char *first, char *second) {
  int length = min(strlen(first), strlen(second));
  for (int position = 0; position < length; position++) {
    if (first[position] > second[position]) {
      return false;
    }
    if (first[position] < second[position]) {
      return true;
    }
  }
  return (strlen(first) < strlen(second));
}

bool is_alpha(WordList *word_list) {
  for (int index = 0; index < word_list->length - 1; index++) {
    if (!sort_alpha(word_list->el[index], word_list->el[index + 1])) {
      return false;
    }
  }
  return true;
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
int find_place(WordList *word_list, char line[LINE_LENGTH]) {
  if (word_list->length == 0) {
    return 0;
  }
  if (sort_alpha(line, word_list->el[0])) {
    return 0;
  }
  for (int index = 0; index < word_list->length - 1; index++) {
    if (sort_alpha(word_list->el[index], line) &&
	sort_alpha(line, word_list->el[index + 1])) {
      return index + 1;
    }
  }
  return word_list->length;
}

void add_to_list(WordList *word_list, char line[LINE_LENGTH]) {
  int position = find_place(word_list, line);
  word_list->el[word_list->length++] = (char *) malloc(LINE_LENGTH + 1);
  for (int later_index = word_list->length - 1; later_index > position; later_index--) {
    strcpy(word_list->el[later_index], word_list->el[later_index - 1]);
    /* word_list->el[later_index] = word_list->el[later_index - 1]; */
  }
  /* word_list->el[position] = line; */
  strcpy(word_list->el[position], line);
}
// load:  22:26 - 23:10ish
WordList read_words_from_file(char *file_name) {
  WordList word_list;
  word_list.el = (char **) malloc(FILE_LENGTH * sizeof(char *));
  word_list.length = 0;
  FILE *file = fopen(file_name, "r");
  if(file ==NULL){
    fprintf(stderr, "Cannot open file?\n");
    exit(EXIT_FAILURE);
  }
  char line[LINE_LENGTH];
  while (fgets(line, LINE_LENGTH, file) != NULL) {
    add_to_list(&word_list, line);
    if(word_list.length == FILE_LENGTH) {
      break;
    }
    if (word_list.length % 1000 == 0) {
      printf("Loaded %i\n", word_list.length);
    }
  }
  fclose(file);
  printf("Loaded %i words\n", word_list.length);
  return word_list;
}

bool str_equal(char first[LINE_LENGTH], char second[LINE_LENGTH]) {
  for (int position = 0; position < max(strlen(first), strlen(second)); position++) {
    if (first[position] == '\n' || second[position] == '\n') {
      continue;
    }
    if (first[position] != second[position]) {
      return false;
    }
  }
  return true;
}

// Searching is super fast
int find_word_index(WordList *word_list, char word[LINE_LENGTH]) {
  for (int position = 0; position < word_list->length; position++) {
    if (str_equal(word_list->el[position], word)) {
      return position;
    }
  }
  return -1;
}

void search_prompt(WordList *word_list) {
  while (true) {
    printf("word search: ");
    char word[LINE_LENGTH];
    scanf("%s", word);
    int matching_index = find_word_index(word_list, word);
    if (matching_index == -1) {
      printf("Could not find %s in the list\n", word);
    } else {
      printf("%s found in the list at position %i\n", word, matching_index);
    }
    printf("Continue? [Y/n] ");
    char continue_response;
    scanf("%s", &continue_response);	  
    if (continue_response == 'n') {
      return;
    }
  }
}

int main() {
  WordList word_list = read_words_from_file(FILENAME);
  search_prompt(&word_list);
  free(word_list.el);
  return 0;
}
