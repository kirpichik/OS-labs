
#include <errno.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>

#include "sync_common.h"
#include "sync_prim.h"

sem_t parent;
sem_t child;

void sync_init() {
  if (sem_init(&parent, 0, 1)) {
    perror("Cannot create parent semaphore");
    exit(errno);
  }

  if (sem_init(&child, 0, 1)) {
    perror("Cannot create child semaphore");
    sem_destroy(&parent);
    exit(errno);
  }
}

#define SEM_ERROR(msg) \
  {                    \
    perror(msg);       \
    sync_destroy();    \
    exit(errno);       \
  }

void sync_parent_prelock() {
  if (sem_wait(&parent))
    SEM_ERROR("Cannot wait parent semaphore in prelock");
  if (sem_wait(&child))
    SEM_ERROR("Cannot wait child semaphore in prelock");
}

void sync_child_prelock() {}

void sync_parent_postunlock() {}

void sync_child_postunlock() {}

void sync_parent_lock() {
  if (sem_wait(&parent))
    SEM_ERROR("Cannot wait parent semaphore");
}

void sync_parent_unlock() {
  if (sem_post(&child))
    SEM_ERROR("Cannot post child semaphore");
}

void sync_child_lock() {
  if (sem_wait(&child))
    SEM_ERROR("Cannot wait child semaphore");
}

void sync_child_unlock() {
  if (sem_post(&parent))
    SEM_ERROR("Cannot post parent semaphore");
}

#undef SEM_ERROR

void sync_destroy() {
  errno = 0;

  if (sem_destroy(&child))
    perror("Cannot destroy child semaphore");

  if (sem_destroy(&parent))
    perror("Cannot destroy parent semaphore");

  if (errno != 0)
    exit(errno);
}
