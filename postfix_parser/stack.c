#include "stack.h"

Stack *make_stack(void) {
  Stack *stack = (Stack *) malloc(sizeof(Stack));
  if (stack == NULL) {
    exit(EXIT_FAILURE);
  }
  return stack;
}

bool stack_push(Stack *stack, void *value, size_t size) {
  if (stack == NULL || value == NULL) {
    return false;
  }
  LinkedList *new_top = (LinkedList *) malloc(sizeof(LinkedList));
  new_top->value = malloc(size);
  memcpy(new_top->value, value, size);
  new_top->next = stack->top;
  stack->top = new_top;  
  return true;
}

bool stack_peek(void *value, Stack *stack, size_t size) {
  if (stack == NULL) {
    return false;
  }
  if (stack->top == NULL) {
    return false;
  }
  if (stack->top->value == NULL) {
    return false;
  }
  memcpy(value, stack->top->value, size);
  return true;
}

bool stack_pop(void *value, Stack *stack, size_t size) {
  if (!stack_peek(value, stack, size)) {
    return false;
  }
  stack->top = stack->top->next;
  // free
  return true;
}

int main(void) {
  Stack *stack = make_stack();
  int val = 0;
  stack_push(stack, &val, sizeof(int));
  val = 99;
  stack_push(stack, &val, sizeof(int));
  int *output = (int *) calloc(1, sizeof(int));
  int *output2 = (int *) calloc(1, sizeof(int));
  stack_pop(output, stack, sizeof(int));
  stack_peek(output2, stack, sizeof(int));
  printf("output is %i, %i\n", *output, *output2);
  
  return 0;
}
