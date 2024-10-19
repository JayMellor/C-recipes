#include "binary_tree.h"

void test(void);

int main(void) {
  test();
  BinaryTree *tree = make_binary_tree();
  int value = 1;
  binary_tree_add(tree, &value, sizeof(int));
  binary_tree_print(tree, &print_number);
  printf("\n");
  return 0;
}

void test(void) {
  BinaryTree *tree = make_binary_tree();
  assert(binary_tree_add(tree, "fred", 5));
  assert(strcmp(tree->value, "fred") == 0);
  
  BinaryTree *child = make_binary_tree();
  binary_tree_add(child, "Hedgehog", 9);
  binary_tree_add_child(tree, Right, child);
  assert(tree->right != NULL);
  assert(strcmp(tree->right->value, "Hedgehog") == 0);
  
  BinaryTree *r_child = make_binary_tree();
  binary_tree_add(r_child, "Filibuster", 11);
  binary_tree_add_child(tree->right, Right, r_child);
  BinaryTree *l_child = make_binary_tree();
  binary_tree_add(l_child, "LED Lights", 11);
  binary_tree_add_child(tree->right, Left, l_child);
  assert(binary_tree_contains(tree, &is_str_equal, "LED Lights"));
  assert(!binary_tree_contains(tree, &is_str_equal, "LCD Lights"));

  binary_tree_print(tree, &print_string);
  printf("\n");
  
  binary_tree_free(tree);
}
