
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sync_common.h"

#define BUFFER_SIZE 4096

static void vsync_error(int err, const char* format, va_list args) {
  char buffer[BUFFER_SIZE + 1];
  int size = vsnprintf(buffer, BUFFER_SIZE, format, args);
  buffer[size] = '\0';
  if (err) {
    fprintf(stderr, "%s: %s\n", buffer, strerror(err));
  } else {
    fprintf(stderr, "%s\n", buffer);
  }
}

void sync_error(int err, const char* format, ...) {
  va_list args;
  va_start(args, format);
  vsync_error(err, format, args);
  va_end(args);
}

void sync_errorcheck_mutex_init(pthread_mutex_t* mutex) {
  pthread_mutexattr_t attr;
  int error;

  if ((error = pthread_mutexattr_init(&attr)) != 0) {
    sync_error(error, "Cannot init mutex attrs");
    exit(error);
  }

  error = pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_ERRORCHECK);
  if (error) {
    sync_error(error, "Cannot set mutex attrs type");
    exit(error);
  }

  if ((error = pthread_mutex_init(mutex, &attr)) != 0) {
    sync_error(error, "Cannot init mutex");
    exit(error);
  }
}

void sync_mutex_lock(pthread_mutex_t* mutex) {
  int error;
  if ((error = pthread_mutex_lock(mutex)) != 0) {
    sync_error(error, "Cannot lock mutex");
    exit(error);
  }
}

void sync_mutex_unlock(pthread_mutex_t* mutex) {
  int error;
  if ((error = pthread_mutex_unlock(mutex)) != 0) {
    sync_error(error, "Cannot unlock mutex");
    exit(error);
  }
}

void sync_mutex_destroy(pthread_mutex_t* mutex) {
  int error;
  if ((error = pthread_mutex_destroy(mutex)) != 0) {
    sync_error(error, "Cannot destroy mutex");
    exit(error);
  }
}
