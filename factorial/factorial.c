#include <stdio.h>

int factorial(int num) {
  if (num <= 1) {
    return 1;
  }
  return num * factorial(num - 1);
}

int main() {
  printf("5! = %i\n", factorial(5));
  return 0;
}

