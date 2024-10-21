#ifndef DEEZHTTP_MEMUTILS_H_
#define DEEZHTTP_MEMUTILS_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "error.h"

inline char* copy_str(const char* str) {
  size_t len = strlen(str);
  char* new_str = (char*)malloc(len + 1);
  CHECK_MALLOC(new_str);
  memcpy(new_str, str, len);
  new_str[len] = '\0';
  return new_str;
}

inline void* copy_obj(const void* obj, size_t size) {
  void* new_obj = malloc(size);
  CHECK_MALLOC(new_obj);
  memcpy(new_obj, obj, size);
  return new_obj;
}

#endif  // DEEZHTTP_MEMUTILS_H_