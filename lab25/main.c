//
// Lab work #25, Process pipe connection
//

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define READ 0
#define WRITE 1

int writerFork(int[2]);
int readerFork(int[2]);

int main() {
  int pipeDesc[2];

  // Creating processes pipe
  if (pipe(pipeDesc) != 0) {
    perror("Pipe error");
    return -1;
  }

  // Creating writer fork
  if (writerFork(pipeDesc) != 0) {
    return -1;
  }

  // Creating reader fork
  if (readerFork(pipeDesc) != 0) {
    return -1;
  }

  close(pipeDesc[READ]);
  close(pipeDesc[WRITE]);

  int status;
  while (wait(&status) != -1) {
    if ((WIFEXITED(status) && WEXITSTATUS(status) != 0) ||
        WIFSIGNALED(status)) {
      return -1;
    }
  }

  return 0;
}

/**
 * Writer fork of process.
 * Reads user input and writes to pipe.
 */
int writerFork(int pipeDesc[2]) {
  ssize_t code;
  char buffer;
  pid_t pid = fork();

  if (pid == -1) {
    perror("Cannot create writer fork");
    return -1;
  }

  if (pid != 0)
    return 0;

  printf("Enter your text here:\n");

  close(pipeDesc[READ]);

  while ((code = read(STDIN_FILENO, &buffer, sizeof(char))) > 0) {
    if (buffer == '\n')
      break;

    if (write(pipeDesc[WRITE], &buffer, sizeof(char)) == -1) {
      perror("Cannot write to pipe");
      exit(-1);
    }
  }

  if (code == -1) {
    perror("Read input error");
    exit(-1);
  }

  close(pipeDesc[WRITE]);
  exit(0);
}

/**
 * Reader fork of process.
 * Reads pipe and writes it to stdout.
 */
int readerFork(int pipeDesc[2]) {
  ssize_t code;
  char buffer;
  pid_t pid = fork();

  if (pid == -1) {
    perror("Cannot create reader fork");
    return -1;
  }

  if (pid != 0)
    return 0;

  close(pipeDesc[WRITE]);

  while ((code = read(pipeDesc[READ], &buffer, sizeof(char))) > 0) {
    buffer = (char)toupper(buffer);
    if (write(STDOUT_FILENO, &buffer, sizeof(char)) == -1) {
      perror("Cannot write to stdout");
      exit(-1);
    }
  }

  if (code == -1) {
    perror("Cannot read pipe");
    exit(-1);
  }

  if (write(STDOUT_FILENO, "\n", sizeof(char)) == -1) {
    perror("Cannot write end of line to stdout");
    exit(-1);
  }

  close(pipeDesc[WRITE]);
  exit(0);
}
