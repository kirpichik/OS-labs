//
// Lab work #16-20, Sync lists
//

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "sync_common.h"
#include "sync_list.h"

#define MAX_LINE_LEN 80
#define SORT_DELAY 5

void* sort_routine(void* arg) {
  sync_list_t* list = (sync_list_t*)arg;
  int error;

  while (1) {
    sleep(SORT_DELAY);

    printf("Sorting list...");

    if ((error = list_sort(list)) != 0) {
      sync_error(error, "Cannot sort list");
      return NULL;
    }

    printf("Done\n");
  }

  return NULL;
}

void read_user_lines(sync_list_t* list) {
  while (1) {
    char* line = (char*)malloc(MAX_LINE_LEN + 1);
    line = fgets(line, MAX_LINE_LEN, stdin);
    if (line == NULL) {
      perror("Cannot read next line");
      return;
    }

    if (strcmp(line, ".\n") == 0)
      break;

    if (line[0] == '\n') {
      printf("Printing list...\n");
      list_print(list);
      printf("List done.\n");
    } else {
      line[strlen(line) - 1] = '\0';
      list_push_forward(list, line);
    }
  }
}

int main(int argc, char* argv[]) {
  int threads_count = 1;
  pthread_t* threads;
  sync_list_t* list;
  int error;

  if (argc > 1) {
    threads_count = atoi(argv[1]);
  }

  threads = (pthread_t*)malloc(sizeof(pthread_t) * threads_count);
  if (threads == NULL) {
    perror("Cannot allocate threads memory");
    return -1;
  }

  list = list_create();
  if (list == NULL) {
    perror("Cannot create list");
    free(threads);
    return -1;
  }

  for (size_t i = 0; i < threads_count; i++) {
    error = pthread_create(threads + i, NULL, &sort_routine, list);
    if (error) {
      sync_error(error, "Cannot create %lu thread", i);
      return -1;
    }
  }

  read_user_lines(list);

  for (size_t i = 0; i < threads_count; i++) {
    if ((error = pthread_cancel(threads[i])) != 0)
      sync_error(error, "Cannot cancel %lu thread", i);

    if ((error = pthread_join(threads[i], NULL)) != 0)
      sync_error(error, "Cannot join %lu thread", i);
  }

  free(threads);
  list_destroy(list);
  return error;
}
