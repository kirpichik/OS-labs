//
// Lab work #2, Pthread join
//

#include <pthread.h>
#include <stdio.h>
#include <string.h>

void* routine(void* arg) {
  printf("Thread started!\n");

  return NULL;
}

int main(int argc, char* argv[]) {
  pthread_t thread;
  int result;

  if ((result = pthread_create(&thread, NULL, &routine, NULL)) != 0) {
    fprintf(stderr, "Cannot create thread: %s", strerror(result));
    return result;
  }

  if ((result = pthread_join(thread, NULL)) != 0) {
    fprintf(stderr, "Cannot join thread: %s\n", strerror(result));
    return result;
  }

  return 0;
}
