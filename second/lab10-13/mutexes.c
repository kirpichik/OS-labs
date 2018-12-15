
#include "sync_common.h"
#include "sync_prim.h"

pthread_mutex_t mutex1;
pthread_mutex_t mutex2;
pthread_mutex_t mutex3;

void sync_init() {
  sync_errorcheck_mutex_init(&mutex1);
  sync_errorcheck_mutex_init(&mutex2);
  sync_errorcheck_mutex_init(&mutex3);
}

void sync_parent_prelock() {
  sync_mutex_lock(&mutex1);
  sync_mutex_lock(&mutex2);
}

void sync_child_prelock() {
  sync_mutex_lock(&mutex3);
}

void sync_parent_postunlock() {
  sync_mutex_unlock(&mutex1);
  sync_mutex_unlock(&mutex2);
}

void sync_child_postunlock() {
  sync_mutex_unlock(&mutex3);
}

void sync_parent_lock() {
  sync_mutex_lock(&mutex1);
  sync_mutex_unlock(&mutex3);
  sync_mutex_lock(&mutex2);
}

void sync_parent_unlock() {
  sync_mutex_unlock(&mutex1);
  sync_mutex_lock(&mutex3);
  sync_mutex_unlock(&mutex2);
}

void sync_child_lock() {
  sync_mutex_lock(&mutex1);
  sync_mutex_unlock(&mutex3);
  sync_mutex_lock(&mutex2);
}

void sync_child_unlock() {
  sync_mutex_unlock(&mutex1);
  sync_mutex_lock(&mutex3);
  sync_mutex_unlock(&mutex2);
}

void sync_destroy() {
  sync_mutex_destroy(&mutex3);
  sync_mutex_destroy(&mutex2);
  sync_mutex_destroy(&mutex1);
}
