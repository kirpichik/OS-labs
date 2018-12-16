
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
