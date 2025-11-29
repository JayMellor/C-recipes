#include <stdio.h>
#include <string.h>

int split(char output[][5], char *str, char split);

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

void test(void) {
    char myStr[] = "(1 . (2 . 3))";
  char splitted[10][5] = {"", "", "", "", "", "", "", "", "", ""};
  split(splitted, myStr, ' ');
  puts("[");
  for (int idx = 0; idx < 10; idx++) {
    printf("%s,\n", splitted[idx]);
  }
  puts("]");
}
