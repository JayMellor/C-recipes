#include "binary_tree.h"

bool is_str_equal(void *first, void *second);

void print_number(void *number) {
  if (number == NULL) {
    return;
  }
  printf("%i",  *((int *) number));
}

bool is_str_equal(void *first, void *second) {
  return strcmp((char *) first, (char *) second) == 0;
}

void print_string(void *str) {
  if (str == NULL) {
    return;
  }
  printf("%s", (char *) str);
}

BinaryTree *make_binary_tree(void) {
  BinaryTree *tree = (BinaryTree *) calloc(1, sizeof(BinaryTree));
  if (tree == NULL) {
    exit(EXIT_FAILURE);
  }
  return tree;
}

bool binary_tree_add(BinaryTree *tree, void *value, size_t size) {
  if (tree->value != NULL) {
    return false;
  }

  tree->value = malloc(size);
  memcpy(tree->value, value, size);
  return true;
}

bool binary_tree_add_child(BinaryTree *tree, Branch branch, BinaryTree *child) {
  BinaryTree **next = branch == Left ? &tree->left : &tree->right;
  if (*next != NULL) {
    return false;
  }

  *next = child;
  return true;
}


bool binary_tree_contains(BinaryTree *tree,
			  bool (*is_equal) (void *first, void *second),
			  void *value) {
  if (tree == NULL) {
    return false;
  }
  if (tree->value == value || is_equal(tree->value, value)) {
    return true;
  }
  return binary_tree_contains(tree->left, is_equal, value) ||
    binary_tree_contains(tree->right, is_equal, value);
}

// Needs flush to ensure printing
void binary_tree_print(BinaryTree *tree, void (*print_value) (void *value)) {
  if (tree == NULL) {
    return;
  }
  print_value(tree->value);
  printf(" (");
  binary_tree_print(tree->left, print_value);
  printf(") (");
  binary_tree_print(tree->right, print_value);
  printf(") ");
}

void binary_tree_free(BinaryTree *tree) {
  // todo add free function
  free(tree->value);
  if (tree->left != NULL) {
    binary_tree_free(tree->left);
  }
  if (tree->right != NULL) {
    binary_tree_free(tree->right);
  }
}
