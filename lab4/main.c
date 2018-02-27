//
// Lab work #4
//

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "linked_list.h"

int readLine(char*, size_t);
int readLines(List*);
void printForeach(void*);

int main(int argc, char* argv[]) {
  List* list = listNew();
  if (!list) {
    perror(argv[0]);
    return ENOMEM;
  }

  if (readLines(list)) {
    perror(argv[0]);
    return ENOMEM;
  }

  listForeach(list, &printForeach);

  listFree(list, true);

  return 0;
}

int readLine(char* buff, size_t size) {
  char c;
  size_t i = 0;

  for (; i < size - 1; i++) {
    c = getc(stdin);
    if (c == '\n' || c == -1) {
      buff[i] = '\0';
      return i;
    }
    buff[i] = c;
  }
  buff[i] = '\0';
  return i;
}

int readLines(List* list) {
  char buff[BUFSIZ];
  char* line;

  while (1) {
    printf(">> ");
    if (!readLine(buff, BUFSIZ))
      break;

    if (buff[0] == '.')
      break;

    line = (char*)malloc(sizeof(char) * (strlen(buff) + 1));
    strcpy(line, buff);

    if (listPushBack(list, line))
      return ENOMEM;
  }

  return 0;
}

void printForeach(void* line) {
  puts((char*)line);
}
