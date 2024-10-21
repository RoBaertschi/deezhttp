#include "error.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

void panic(const char* format, ...) {
    va_list list;
    va_start(list, format);
    vfprintf(stderr, format, list);
    va_end(list);
    abort();
}
