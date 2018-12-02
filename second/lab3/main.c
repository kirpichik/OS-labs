//
// Lab work #3, Pthread args
//

#include <pthread.h>
#include <stdio.h>
#include <string.h>

#define THREADS_COUNT 4
#define MAX_STRINGS 16

typedef struct toThread {
  const int number;
  char* strs[MAX_STRINGS];
} toThread;

void* routine(void* arg) {
  char** lines = (char**)arg;

  while ((*lines)) {
    printf("%s\n", (*lines));
    lines++;
  }

  return NULL;
}

int main(int argc, char* argv[]) {
  pthread_t threads[4];
  int result;

  char* args[THREADS_COUNT][MAX_STRINGS] = {
      {"First", "thread", NULL},
      {"My", "second", "thread", NULL},
      {"My", "mega", "third", "thread", NULL},
      {"Thread", NULL}
  };

  for (size_t i = 0; i < THREADS_COUNT; i++)
    if ((result = pthread_create(&threads[i], NULL, &routine, &args[i])) != 0) {
      fprintf(stderr, "Cannot create thread: %s\n", strerror(result));
      return result;
    }

  for (size_t i = 0; i < THREADS_COUNT; ++i)
    if ((result = pthread_join(threads[i], NULL)) != 0) {
      fprintf(stderr, "Cannot join thread: %s", strerror(result));
      return result;
    }

  return 0;
}
