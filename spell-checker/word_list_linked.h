#pragma once
#include "word_list.h"
#include "../generic_linked_list/linked_list.h"

struct WordList {
  linked_list *first;
  linked_list *last;  
  int length;
};
