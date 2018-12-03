//
// Lab work #7, Count pi
//

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ITERATIONS 20000000

typedef struct pi_state {
  size_t pos;
  size_t amount;
  pthread_t thread;
  double result;
} pi_state_t;

void* count_pi(void* arg) {
  pi_state_t* state = (pi_state_t*)arg;

  state->result = 0.0;
  for (size_t i = state->pos; i <= ITERATIONS; i += state->amount) {
    state->result += 1.0 / (i * 4.0 + 1.0);
    state->result -= 1.0 / (i * 4.0 + 3.0);
  }

  return &state->result;
}

int main(int argc, char* argv[]) {
  pi_state_t* states;
  int threads_count;
  double pi = 0.;
  void* value;
  int result;

  if (argc < 2) {
    fprintf(stderr, "Need args: <threads>\n");
    return -1;
  }

  threads_count = atoi(argv[1]);
  if (threads_count < 1) {
    fprintf(stderr, "Wrong threads number\n");
    return -1;
  }

  states = (pi_state_t*)malloc(sizeof(pi_state_t) * threads_count);

  for (size_t i = 0; i < threads_count; i++) {
    states[i].pos = i;
    states[i].amount = threads_count;

    result = pthread_create(&states[i].thread, NULL, &count_pi, states + i);
    if (result != 0) {
      fprintf(stderr, "Cannot create thread: %s\n", strerror(result));
      free(states);
      return result;
    }
  }

  for (size_t i = 0; i < threads_count; i++) {
    if ((result = pthread_join(states[i].thread, &value)) != 0) {
      fprintf(stderr, "Cannot join thread: %s\n", strerror(result));
      free(states);
      return result;
    }

    pi += (*((double*)value));
  }
  free(states);

  pi *= 4;
  printf("pi: %.15lf\n", pi);
  return 0;
}
