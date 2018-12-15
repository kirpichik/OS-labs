
#ifndef SYNC_COMMON_H
#define SYNC_COMMON_H

#include <pthread.h>

void sync_error(int err, const char* format, ...);

void sync_errorcheck_mutex_init(pthread_mutex_t* mutex);

void sync_mutex_lock(pthread_mutex_t* mutex);

void sync_mutex_unlock(pthread_mutex_t* mutex);

void sync_mutex_destroy(pthread_mutex_t* mutex);

#endif
