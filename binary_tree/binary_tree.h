#pragma once
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>

typedef struct BinaryTree {
  void *value;
  struct BinaryTree *left;
  struct BinaryTree *right;
} BinaryTree;

typedef enum Branch {
  Left,
  Right
} Branch;

BinaryTree *make_binary_tree();
bool binary_tree_add(BinaryTree *tree, void *value, size_t size);
// Could have something like ..._add(tree->left, child) and use the pointer?
bool binary_tree_add_child(BinaryTree *tree, Branch branch, BinaryTree *child);
bool binary_tree_remove(BinaryTree *tree); // ?
bool binary_tree_contains(BinaryTree *tree,
			  bool (*is_equal) (void *first, void *second),
			  void *value);
void binary_tree_print(BinaryTree *tree, void (*print_value) (void *value));
void binary_tree_free(BinaryTree *tree);
void test();
