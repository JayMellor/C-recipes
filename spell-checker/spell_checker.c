#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
/* #include "word_list_array.h" */
#include "word_list_linked.h"

#define FILENAME "./eng_370k_shuffle.txt"

WordList *read_words_from_file(char *file_name);
void search_prompt(WordList *word_list);
void strip_string(char *str);

int main(int argc, char *argv[]) {
  if (argc >= 2 && strcmp(argv[1], "run") == 0) {
    WordList *word_list = read_words_from_file(FILENAME);
    printf("list %s\n", word_list_is_sorted(word_list) ? "is sorted" : "isn't sorted");
    search_prompt(word_list);
    word_list_free(word_list);
    return 0;
  } else {
    test();
    return 0;  
  }
}

// load linear with array:  22:26 - 23:10ish
// load linear with end check: 17:58 - 18:14
// load bin with array: 17:42 - 17:55
// load with linked list 23:05 - 23:10
WordList *read_words_from_file(char *file_name) {
  WordList *word_list = word_list_init();
  FILE *file = fopen(file_name, "r");
  if(file ==NULL){
    fprintf(stderr, "Cannot open file?\n");
    exit(EXIT_FAILURE);
  }
  char line[LINE_LENGTH];
  while (fgets(line, LINE_LENGTH, file) != NULL) {
    strip_string(line);
    word_list_add(word_list, line);
    if (word_list->length % 1000 == 0) {
      printf("Loaded %i\n", word_list->length);
    }
  }
  fclose(file);
  time_t now;
  time(&now);
  printf("Loaded %i words. Finished at %s\n", word_list->length, ctime(&now));
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

/** Strips a string of spaces.
 *  STR should not be dynamically allocated or static.
 */ 
void strip_string(char *str) {
  for (int position = 0; position < strlen(str); position++) {
    if (isspace(str[position])) {
      str[position] = '\0';
      return;
    }
  }
  
}
