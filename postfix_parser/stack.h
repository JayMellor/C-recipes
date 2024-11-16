#pragma once
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
/* #include "../generic_linked_list/linked_list.h" */

typedef struct LinkedList {
  void *value;
  struct LinkedList *next;
} LinkedList;

typedef struct Stack {
  LinkedList *top;
} Stack;

Stack *make_stack(void);
bool stack_push(Stack *stack, void *value, size_t size);
bool stack_pop(void *value, Stack *stack, size_t size);
bool stack_peek(void *value, Stack *stack, size_t size);
void stack_free(Stack *stack);
