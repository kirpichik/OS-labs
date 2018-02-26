//
// Лабораторная работа #3, UID файла.
//

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main(int argc, char* argv[]) {
  if (argc != 2) {
    printf("Need argument: <filename>\n");
    return 1;
  }

  printf("Real uid: ", getuid());
  return 0;
}

