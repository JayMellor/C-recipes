#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "word_list_array.h"

#define FILENAME "./eng_370k_shuffle.txt"

WordList *read_words_from_file(char *file_name);
void search_prompt(WordList *word_list);

int main() {
  test();
  WordList *word_list = read_words_from_file(FILENAME);
  search_prompt(word_list);
  word_list_free(word_list);
  return 0;
}

// load with array:  22:26 - 23:10ish
WordList *read_words_from_file(char *file_name) {
  WordList *word_list = word_list_init();
  FILE *file = fopen(file_name, "r");
  if(file ==NULL){
    fprintf(stderr, "Cannot open file?\n");
    exit(EXIT_FAILURE);
  }
  char line[LINE_LENGTH];
  while (fgets(line, LINE_LENGTH, file) != NULL) {
    word_list_add(word_list, line);
    if(word_list->length == 2000) {
      break;
    }
    if (word_list->length % 1000 == 0) {
      printf("Loaded %i\n", word_list->length);
    }
  }
  fclose(file);
  printf("Loaded %i words\n", word_list->length);
  return word_list;
}

void search_prompt(WordList *word_list) {
  while (true) {
    printf("word search: ");
    char word[LINE_LENGTH];
    scanf("%s", word);
    char *matching_word = word_list_find(word_list, word);
    if (matching_word == NULL) {
      printf("Could not find %s in the list\n", word);
    } else {
      printf("%s found in the list at position\n", word);
    }
    printf("Continue? [Y/n] ");
    char continue_response;
    scanf("%s", &continue_response);	  
    if (continue_response == 'n') {
      return;
    }
  }
}
