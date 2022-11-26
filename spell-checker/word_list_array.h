#pragma once
#include "word_list.h"
#define CHUNK_SIZE 100

struct WordList {
  char **el;
  int length;
};
