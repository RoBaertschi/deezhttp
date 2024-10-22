#ifndef DEEZHTTP_ERROR_H_
#define DEEZHTTP_ERROR_H_

#include <errno.h>
#include <string.h>

void panic(const char* format, ...);

#define CHECK_MALLOC(variable)                                                 \
  if (variable == NULL)                                                        \
  panic(__FILE__ ":%d deezhttp ran out of memory / alloc failed %s", __LINE__, \
        strerror(errno))
#define CHECK_FD(fd)                                                \
  if (fd == -1)                                                     \
  panic(__FILE__ ":%d deezhttp file descriptor error %s", __LINE__, \
        strerror(errno))
#define CHECK_FREED(object) \
  if (object != NULL) free(object)

#endif  // DEEZHTTP_ERROR_H_
