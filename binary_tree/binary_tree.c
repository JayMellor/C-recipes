#include "binary_tree.h"

bool is_str_equal(void *first, void *second) {
  return strcmp((char *) first, (char *) second) == 0;
}

void test() {
  BinaryTree *tree = make_binary_tree();
  binary_tree_add(tree, "fred", 5);
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
  
  binary_tree_free(tree);
}

int main() {
  test();
  return 0;
}

BinaryTree *make_binary_tree() {
   return (BinaryTree *) malloc(sizeof(BinaryTree));
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
