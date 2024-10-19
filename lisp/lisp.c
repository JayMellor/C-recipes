#include "lisp.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../binary_tree/binary_tree.h"

#define NIL "NIL"

void cons_to_string(char *str, Cons *cons);
int split(char output[][5], char *str, char split);

void assert_equal(char *expected, char *actual) {
  if ((expected == NULL) && actual != NULL) {
    printf("'%s' does not equal '%s'\n", expected, actual);
    exit(EXIT_FAILURE);
  }

  if ((actual == NULL) && expected != NULL) {
    printf("'%s' does not equal '%s'\n", expected, actual);
    exit(EXIT_FAILURE);    
  }

  if (strcmp(expected, actual) != 0) {
    printf("'%s' does not equal '%s'\n", expected, actual);
    exit(EXIT_FAILURE);
  }

  return;
}

void strncpy_test() {
  char str1[] = "alphabetic";
  char sub[5] = "";
  strncpy(sub, str1, 4);
  puts(sub);
}

void test(void) {
    Cons *myCons = cons(atom("first", sizeof("first")), cons(atom("second", sizeof("second")), NULL));
	char myConsStr[50] = "";
	cons_to_string(myConsStr, myCons);
	assert_equal(myConsStr, "(first . (second))");

	Cons *sCons = cons(NULL, NULL);
	cons_to_string(myConsStr, sCons);
	char expected[6] = "";
	snprintf(expected, 6, "(%s)", NIL);
	assert_equal(myConsStr, expected);

	Cons *tCons =
	  cons(atom("1", sizeof("1")), cons(atom("2", sizeof("2")), NULL));
	cons_to_string(myConsStr, tCons);
	assert_equal(myConsStr, "(1 . (2))");

	Cons *fCons = cons(atom("3", sizeof("3")), NULL);
	cons_to_string(myConsStr, fCons);
	assert_equal(myConsStr, "(3)");
        
	Cons *matom = atom("2", sizeof("2"));
	cons_to_string(myConsStr, matom);
	assert_equal(myConsStr, "2");
        
	delete_cons(myCons);
	delete_cons(sCons);
	delete_cons(tCons);
}

int main(void) {  
  // (DEFINE MYVAR 2)
  // 2
  // -> 2
  // (1 2 3)
  // (1 . (2 . (3)))
  // cons(1, cons(2, cons(3, NULL)))
        /* test(); */

  parse("(CONS 1 2)");
  return 0;

  char myStr[] = "(1 . (2 . 3))";
  char splitted[10][5] = {"", "", "", "", "", "", "", "", "", ""};
  split(splitted, myStr, ' ');
  puts("[");
  for (int idx = 0; idx < 10; idx++) {
  	printf("%s,\n",splitted[idx]);
  }
  puts("]");
  return 0;

}

Cons *new_cons(void) {
  Cons *cons = (Cons *) calloc(1, sizeof(Cons)); 
  return cons;
}

Cons *atom(void *value, size_t size) {
  Cons *cons = new_cons();
  cons->car = NULL;
  cons->cdr = NULL;
  cons->atom = (void *) malloc(size);
  cons->size = size;
  memcpy(cons->atom, value, size);
  return cons;
}

Cons *cons(Cons *car, Cons *cdr) {
  Cons *cons = new_cons();
  cons->car = car;
  cons->cdr = cdr;
  cons->atom = NULL;
  cons->size = 0;
  return cons;
}

bool is_atomic(Cons *cons) { return cons->atom != NULL; }

void cons_to_string(char *str, Cons *cons) {
  if (cons == NULL) {
    strcpy(str, "");
	return;
  }

  if (is_atomic(cons)) {
	strcpy(str, cons->atom);
	return;
  }

  char car_str[50];
  cons_to_string(car_str, cons->car);
  if (strlen(car_str) == 0) {
	strcpy(car_str, NIL);
  }
  
  char cdr_str[50];
  cons_to_string(cdr_str, cons->cdr);

  if (strlen(cdr_str) == 0) {
	snprintf(str, sizeof(str)+2, "(%s)", car_str);
	return;
  }  

  snprintf(str,strlen(car_str) + strlen(cdr_str) + 10, "(%s . %s)", car_str, cdr_str);
  return;
}

void print_cons(Cons *cons) {
  char str[50];
  cons_to_string(str, cons);
  puts(str); 
}

void delete_cons(Cons *cons) {
  if (cons == NULL) {
	return;
  }

  if (cons->atom != NULL) {
	free(cons->atom);
	cons->atom = NULL;
  }

  if (cons->car != NULL) {
	delete_cons(cons->car);
  }

  if (cons->cdr != NULL) {
	delete_cons(cons->cdr);
  }

  free(cons);
  cons = NULL;
}

int find(char *str, char ch) {
  for (int idx = 0; idx < strlen(str); idx++) {
	if (str[idx] == ch) {
	  return idx;
	}
  }
  return -1;
}

int split(char output[][5], char *str, char splitChar) {
  int chunkStart = 0;
  int chunkCount = 0;
  for (int idx = 0; idx < strlen(str); idx++) {
	if (str[idx] == splitChar) {
	  strncpy(output[chunkCount], str + chunkStart, idx - chunkStart);
	  chunkCount++;
	  chunkStart = idx;
    }
  }
  strncpy(output[chunkCount], str + chunkStart, strlen(str) - chunkStart);
  return chunkCount;  
}

// (CONS 1 (CONS 2 3))
// (+ 1 2 3) => (+ 1 (2 3))
// (LAMBDA (first second) (+ first second))
BinaryTree *parseToTree(char *input) {
  BinaryTree *tree = make_binary_tree();

  bool afterOpenBracket = false;
  Branch branch = Left;
  char buffer[10] = "";
  int bufferIdx = 0;
  for (int idx = 0; idx < strlen(input); idx++) {
	/* printf("buffer: (%s), bufferIdx: %d, current: %c\n", buffer, bufferIdx, input[idx]); */
    if ((input[idx] == ' ') || (input[idx] == ')')) {
	  if (afterOpenBracket) {
		binary_tree_add(tree, buffer, strlen(buffer));

		afterOpenBracket = false;
	  } else {
		BinaryTree *child = make_binary_tree();
		binary_tree_add(child, buffer, strlen(buffer));
		binary_tree_add_child(tree, branch, child);

		if (branch == Left) {
		  branch = Right;
		} else {
		  branch = Left;
		}
	  }
          
	  bufferIdx = 0;
	  for (int ii = 0; ii < 9; ii++) {
		buffer[ii] = 0;
	  }
	  buffer[9] = '\0';
	  
	  continue;
    }
    if (input[idx] == '(') {
	  afterOpenBracket = true;
	  continue;
    }
	buffer[bufferIdx] = input[idx];
	bufferIdx++;	
  }
  return tree;
}

Cons *parse(char *input) {
  BinaryTree *tree = parseToTree(input);
  printf("is tree null? %s\n", tree == NULL ? "yes" : "no");
  printf("(%s (%s) (%s))\n", tree->value,tree->left == NULL ? "null" : tree->left->value, tree->right == NULL ? "null": tree->right->value);
  binary_tree_print(tree, &print_string);
}

/* Cons *parse(char *input) { */
/*   if (strlen(input) == 0) { */
/* 	return NULL; */
/*   } */

/*   if (input[0] != '(') { */
/* 	// symbol outside cons */
/* 	return NULL; */
/*   } */

/*   int nextCloseBracket = find(input, ')'); */
/*   int nextOpenBracket = find(input, '('); */

/*   if (nextCloseBracket == -1) { */
/* 	// imbalanced brackets */
/* 	return NULL; */
/*   } */

/*   if (nextOpenBracket == -1) { */
/* 	// expression in single cons */
/*   } */

/*   if (nextOpenBracket > nextCloseBracket) { */
/* 	// can ignore open bracket */
/*   } */

/*   if (nextOpenBracket < nextCloseBracket) { */
/* 	// nesting */
/*   } */
  
/* } */
