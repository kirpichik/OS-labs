//
// Lab work #22, Widget factory
//

#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

sem_t sem_a, sem_b, sem_c, sem_ab;

typedef struct worker {
  char* name;
  unsigned char delay;
  sem_t* sem_depend_1;
  sem_t* sem_depend_2;
  sem_t* sem_out;
} worker_t;

void* worker_routine(void* arg) {
  worker_t* worker = (worker_t*)arg;
  sem_t* sem_depends[2] = {worker->sem_depend_1, worker->sem_depend_2};
  char count = worker->sem_depend_2 ? 2 : worker->sem_depend_1 ? 1 : 0;

  while (1) {
    sleep(worker->delay);
    for (char i = 0; i < count; i++) {
      if (0 != sem_wait(sem_depends[i])) {
        perror("Cannot wait semaphore");
        exit(-1);
      }
    }

    printf("[%s] Created detail\n", worker->name);

    if (!worker->sem_out)
      continue;
    if (0 != sem_post(worker->sem_out)) {
      perror("Cannot post semaphore");
      exit(-1);
    }
  }
}

void sem_try_init(sem_t* sem) {
  if (sem_init(sem, 0, 0)) {
    perror("Cannot init semaphore");
    exit(-1);
  }
}

int main() {
  pthread_t threads[5];
  worker_t workers[5] = {
      {"A", 1, NULL, NULL, &sem_a},
      {"B", 2, NULL, NULL, &sem_b},
      {"C", 3, NULL, NULL, &sem_c},
      {"AB", 0, &sem_a, &sem_b, &sem_ab},
      {"WIDGET", 0, &sem_ab, &sem_c, NULL}};

  sem_try_init(&sem_a);
  sem_try_init(&sem_b);
  sem_try_init(&sem_c);
  sem_try_init(&sem_ab);

  for (unsigned char i = 0; i < 5; i++)
    if (pthread_create(&threads[i], NULL, &worker_routine, &workers[i])) {
      perror("Cannot create worker thread");
      return -1;
    }

  pthread_exit(NULL);
}
