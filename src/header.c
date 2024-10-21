#include "header.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char* dh_header_field_to_string(dh_header_field* field) {
  size_t len = field->key_len + field->value_len + 4;
  char* str = malloc(len);
  if (str == NULL) {
    return NULL;
  }

  snprintf(str, len, "%s: %s", field->key, field->value);
  return str;
};
