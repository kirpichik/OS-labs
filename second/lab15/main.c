//
// Lab work #15, Sync output, processes
//

#include <errno.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>

#define PRINT_COUNT 10

#define SEM_PARENT "/parent"
#define SEM_CHILD "/child"

static void open_semaphores(sem_t** parent, sem_t** child) {
  if ((*parent = sem_open(SEM_PARENT, O_RDWR)) == SEM_FAILED) {
    perror("Cannot open parent semaphore");
    exit(errno);
  }

  if ((*child = sem_open(SEM_PARENT, O_RDWR)) == SEM_FAILED) {
    perror("Cannot open child semaphore");
    exit(errno);
  }
}

static void print_parent() {
  sem_t* parent;
  sem_t* child;

  open_semaphores(&parent, &child);

  for (size_t i = 0; i < PRINT_COUNT; i++) {
    sem_wait(parent);

    printf("Parent output\n");
    fflush(stdout);

    sem_post(child);
  }
}

static void print_child() {
  sem_t* parent;
  sem_t* child;

  open_semaphores(&parent, &child);

  for (size_t i = 0; i < PRINT_COUNT; i++) {
    sem_wait(child);

    printf("Child output\n");
    fflush(stdout);

    sem_post(parent);
  }
}

static void delete_semaphores() {
  if (sem_unlink(SEM_CHILD) && errno != ENOENT)
    perror("Cannot unlink child semaphore");

  if (sem_unlink(SEM_PARENT) && errno != ENOENT)
    perror("Cannot unlink parent semaphore");
}

static void init_semaphores() {
  int flags = O_CREAT | O_EXCL;
  mode_t mode = S_IRWXU | S_IRWXG | S_IRWXO;
  sem_t* parent;
  sem_t* child;

  if ((parent = sem_open(SEM_PARENT, flags, mode, 1)) == SEM_FAILED) {
    perror("Cannot create parent semaphore");
    exit(errno);
  }

  if ((child = sem_open(SEM_PARENT, flags, mode, 1)) == SEM_FAILED) {
    perror("Cannot create child semaphore");
    exit(errno);
  }

  sem_close(parent);
  sem_close(child);
}

int main(int argc, char* argv[]) {
  pid_t pid;

  delete_semaphores();
  init_semaphores();
  atexit(&delete_semaphores);

  if ((pid = fork()) == -1) {
    perror("Cannot create child process");
    exit(errno);
  }

  if (pid == 0)
    print_child();
  else {
    print_parent();

    if (wait(NULL) == -1) {
      perror("Cannot wait for child process");
      exit(errno);
    }
  }

  exit(0);
}
