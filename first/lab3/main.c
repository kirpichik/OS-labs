//
// Lab work #3, UID of file.
//

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
  if (argc != 2) {
    printf("Need argument: <filename>\n");
    return 1;
  }

  printf("Real uid: ", getuid());
  return 0;
}
