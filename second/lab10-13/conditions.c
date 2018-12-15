
#include <stdbool.h>
#include <stdlib.h>

#include "sync_common.h"
#include "sync_prim.h"

pthread_mutex_t mutex;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
bool child_next = false;

void sync_init() {
  sync_errorcheck_mutex_init(&mutex);
}

void sync_parent_prelock() {
  sync_mutex_lock(&mutex);
}

void sync_child_prelock() {}

void sync_parent_postunlock() {
  sync_mutex_unlock(&mutex);
}

void sync_child_postunlock() {}

void sync_parent_lock() {
  int error;

  sync_mutex_lock(&mutex);
  // "While" because signal may be sent to self
  while (child_next) {
    if ((error = pthread_cond_wait(&cond, &mutex)) != 0) {
      sync_error(error, "Cannot wait condition");
      exit(error);
    }
  }
}

void sync_parent_unlock() {
  child_next = true;
  pthread_cond_signal(&cond);
  sync_mutex_unlock(&mutex);
}

void sync_child_lock() {
  int error;

  sync_mutex_lock(&mutex);
  // "While" because signal may be sent to self
  while (!child_next) {
    if ((error = pthread_cond_wait(&cond, &mutex)) != 0) {
      sync_error(error, "Cannot wait condition");
      exit(error);
    }
  }
}

void sync_child_unlock() {
  child_next = false;
  pthread_cond_signal(&cond);
  sync_mutex_unlock(&mutex);
}

void sync_destroy() {
  sync_mutex_destroy(&mutex);
}
