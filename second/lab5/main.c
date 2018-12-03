//
// Lab work #5, Pthread cancel handle
//

#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

void cleanup(void* arg) {
  puts("Cancelling...");
}

void* routine(void* arg) {
  pthread_cleanup_push(&cleanup, NULL);

  while (1) {
    puts("Running...");
    pthread_testcancel();
  }

  pthread_cleanup_pop(1);
  return NULL;
}

int main(int argc, char* argv[]) {
  pthread_t thread;
  int result;
  void* value;

  if ((result = pthread_create(&thread, NULL, &routine, NULL)) != 0) {
    fprintf(stderr, "Cannot create thread: %s\n", strerror(result));
    return result;
  }

  sleep(2);

  if ((result = pthread_cancel(thread)) != 0) {
    fprintf(stderr, "Cannot cancel thread: %s\n", strerror(result));
    return result;
  }

  if ((result = pthread_join(thread, &value)) != 0) {
    fprintf(stderr, "Cannot join thread: %s\n", strerror(result));
    return result;
  }

  if (value == PTHREAD_CANCELED)
    puts("Thread cancelled");
  else
    puts("Thread finished?");

  return 0;
}
