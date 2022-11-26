#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "string.h"
#define LINE_LENGTH 100

typedef struct WordList WordList;

WordList *word_list_init();
void word_list_add(WordList *word_list, char *line);
int word_list_size(WordList *word_list);
char *word_list_find(WordList *word_list, char *word);
void word_list_print(WordList *word_list);
void word_list_free(WordList *word_list);
bool word_list_is_sorted(WordList *word_list);
void test();
