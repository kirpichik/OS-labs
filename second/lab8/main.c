//
// Lab work #8, Inf count pi
//

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <stdbool.h>

#define CHECK_ITER 1000000

typedef struct pi_state {
  size_t pos;
  size_t amount;
  pthread_t thread;
  double result;
} pi_state_t;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
bool finished = false;
size_t max = 0;

void* count_pi(void* arg) {
  pi_state_t* state = (pi_state_t*)arg;

  sigset_t signals;
  sigemptyset(&signals);
  sigaddset(&signals, SIGINT);
  pthread_sigmask(SIG_BLOCK, &signals, NULL);

  size_t step = state->pos;
  size_t i;
  state->result = 0.0;
  for (i = 0; ; i++) {
    state->result += 1.0 / (step * 4.0 + 1.0);
    state->result -= 1.0 / (step * 4.0 + 3.0);
    step += state->amount;

    if (i % CHECK_ITER == 0) {
      pthread_mutex_lock(&mutex);
      if (finished) {
        pthread_mutex_unlock(&mutex);
        break;
      }
      if (max < i + CHECK_ITER)
        max += CHECK_ITER;
      pthread_mutex_unlock(&mutex);
    }
  }

  size_t left = max - i;
  printf("Left: %lu\n", left);
  for (size_t i = 0; i < left; i++) {
    state->result += 1.0 / (step * 4.0 + 1.0);
    state->result -= 1.0 / (step * 4.0 + 3.0);
    step += state->amount;
  }
  printf("Step: %lu, i: %lu\n", step, i);

  return &state->result;
}

void interrupt_handler(int signal) {
  pthread_mutex_lock(&mutex);
  finished = true;
  pthread_mutex_unlock(&mutex);
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

  if (signal(SIGINT, &interrupt_handler) == SIG_ERR) {
    perror("Cannot set interrupt handler");
    return -1;
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

