#ifndef DEEZHTTP_ERROR_H_
#define DEEZHTTP_ERROR_H_

#include <errno.h>
#include <string.h>

void panic(const char* format, ...);

#define CHECK_MALLOC(variable) if (variable == NULL) panic( __FILE__ __LINE__ " deezhttp ran out of memory / alloc failed %s", strerror(errno))

#endif // DEEZHTTP_ERROR_H_
