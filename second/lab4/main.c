//
// Lab work #4, Pthread cancel
//

#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

void* routine(void* arg) {
  while (1) {
    puts("Running...");
    pthread_testcancel();
  }

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
    fprintf(stderr, "Cannot cancel thread^ %s\n", strerror(result));
    return result;
  }

  if ((result = pthread_join(thread, &value)) != 0) {
    fprintf(stderr, "Couldn't join thread: %s\n", strerror(result));
    return result;
  }

  if (value == PTHREAD_CANCELED)
    printf("Thread cancelled\n");
  else
    printf("Thread finished?\n");

  return 0;
}
