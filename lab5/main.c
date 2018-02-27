//
// Lab work #5, Table of text file lines.
//

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define MAX_LINES 100
#define FINISH_NUMBER 0

int prepareTables(int, long*, int*);
int doInput(int, int, long*, int*);
int printLine(int, long, int);

int main(int argc, char* argv[]) {
  long offsets[MAX_LINES + 1];
  int lengths[MAX_LINES + 1];
  int file;

  if (argc != 2) {
    fprintf(stderr, "Usage: %s <file>\n", argv[0]);
    return EINVAL;
  }

  if ((file = open(argv[1], O_RDONLY | O_NDELAY)) == -1) {
    perror(argv[0]);
    return EIO;
  }

  int lines = prepareTables(file, offsets, lengths);
  if (lines == -1)
    return -1;

  return doInput(file, lines, offsets, lengths);
}

/**
 * Prepares offsets and lengths tables.
 *
 * @param file Input file descriptor.
 * @param offsets Pointer to offsets table.
 * @param lens Pointer to lengths table.
 *
 * @return Lines count or -1, if error occuried when read.
 */
int prepareTables(int file, long* offsets, int* lens) {
  char ch;
  int code;
  int currOffset;
  int currLine = 1;  // Zero line is file begin.
  int currPos = 0;

  offsets[1] = 0L;

  while (1) {
    if ((code = read(file, &ch, 1)) == -1) {
      perror("Cannot read file: ");
      return -1;
    }

    // Check EOF
    if (!code)
      break;

    // Check lines count
    if (currLine > MAX_LINES + 1) {
      fprintf(stderr, "File must has less than %d lines.\n", (MAX_LINES + 1));
      return -1;
    }

    currPos++;

    // Wait for line ends
    if (ch != '\n')
      continue;

    lens[currLine] = currPos;
    currLine++;

    if ((currOffset = lseek(file, 0L, SEEK_CUR)) == -1) {
      perror("Cannot seek: ");
      return -1;
    }

    offsets[currLine] = currOffset;
    currPos = 0;
  }
  return currLine - 1;
}

/**
 * Wait for user input and prints requested line from file.
 * FINISH_NUMBER input will finish execution.
 *
 * @param file Input file descriptor.
 * @param lines Lines count in tables.
 * @param offsets Poiter to offsets table.
 * @param lens Poiter to lengths table.
 *
 * @return Exit code of program.
 */
int doInput(int file, int lines, long* offsets, int* lens) {
  int lineNum;
  char end;

  printf("Type line number from 1 to %d\n", lines);

  while (1) {
    printf(">> ");

    // Read number and check formatting
    if (scanf("%d%c", &lineNum, &end) != 2 || end != '\n') {
      fprintf(stderr, "Invalid input.\n");
      return -1;
    }

    if (lineNum == FINISH_NUMBER)
      return 0;

    if (lineNum < 0 || lineNum > lines) {
      fprintf(stderr, "Invalid line position.\n");
      return -1;
    }

    printLine(file, offsets[lineNum], lens[lineNum]);
  }

  return 0;
}

/**
 * Prints line from file using prepared lines table.
 *
 * @param file Input file descriptor.
 * @param offset File offset to line begin.
 * @param len Len of line.
 *
 * @return Zero if line readed and printed success or non-zero, if not.
 */
int printLine(int file, long offset, int len) {
  char c;
  lseek(file, offset, SEEK_SET);

  while (len-- > 0) {
    if (read(file, &c, 1) == -1) {
      perror("Cannot read file: ");
      return -1;
    }

    if (write(STDOUT_FILENO, &c, 1) == -1) {
      perror("Cannot write char: ");
      return -1;
    }
  }

  return 0;
}
