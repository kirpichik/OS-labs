//
// Lab work #10-13, Sync output
//

#include <pthread.h>
#include <stdio.h>

#include "sync_common.h"
#include "sync_prim.h"

#define PRINT_COUNT 10

static void* print_child(void* arg) {
  sync_child_prelock();

  for (size_t i = 0; i < PRINT_COUNT; i++) {
    sync_child_lock();

    printf("Child output\n");
    fflush(stdout);

    sync_child_unlock();
  }

  sync_child_postunlock();
  return NULL;
}

static void print_parent() {
  for (size_t i = 0; i < PRINT_COUNT; i++) {
    printf("Parent output\n");
    fflush(stdout);

    sync_parent_unlock();
    sync_parent_lock();
  }

  sync_parent_postunlock();
}

int main(int argc, char* argv[]) {
  pthread_t thread;
  int error;

  sync_init();
  sync_parent_prelock();

  if ((error = pthread_create(&thread, NULL, &print_child, NULL)) != 0) {
    sync_error(error, "Cannot create thread");
    return error;
  }

  print_parent();

  if ((error = pthread_join(thread, NULL)) != 0) {
    sync_error(error, "Cannot join thread");
    return error;
  }

  sync_destroy();
  return 0;
}
