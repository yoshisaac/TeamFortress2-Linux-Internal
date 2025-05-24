#include <stdio.h>
#include <stdarg.h>
#include <unistd.h>

FILE* log_file = NULL;
void print(const char* fmt, ...) {

  if (!log_file) {
    log_file = fopen("/tmp/tf2.log", "w");
  }

  va_list args;

  va_start(args, fmt);

  vfprintf(log_file, fmt, args);

  fflush(log_file);

  va_end(args);
}
