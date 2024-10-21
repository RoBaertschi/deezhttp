#include "header.h"

#include <stdio.h>
#include <stdlib.h>

#include "error.h"

const char* dh_header_field_to_string(dh_header_field* field) {
  size_t len = field->key_len + field->value_len + 4;
  char* str = malloc(len);
  CHECK_MALLOC(str);

  snprintf(str, len, "%s: %s", field->key, field->value);
  return str;
}

const char* dh_header_field_string(size_t* length, dh_header_field* field) {
  size_t len = field->key_len + field->value_len + 4;
  char* str = malloc(len);
  CHECK_MALLOC(str);

  snprintf(str, len, "  %s: %s", field->key, field->value);
  *length = len;
  return str;
}

inline const char* dh_header_protocol_string(size_t* length,
                                             dh_protocol protocol) {
  switch (protocol) {
    case DH_HTTP_1_0:
      *length = 8;
      return "HTTP/1.0";
    case DH_HTTP_INVALID:
      *length = 7;
      return "INVALID";
  }
}

const char* dh_header_method_string(size_t* length, dh_http_method method) {
  if (method == DH_GET) {
    *length = 3;
    return "GET";
  } else if (method == DH_POST) {
    *length = 4;
    return "POST";
  } else if (method == DH_HEAD) {
    *length = 4;
    return "HEAD";
  }

  *length = 0;
  return 0;
}

const char* dh_header_string(size_t* length, dh_header* header) {
  size_t len;
  size_t method_len;
  const char* method_str = dh_header_method_string(&method_len, header->method);
  size_t protocol_len;
  const char* protocol_str =
    dh_header_protocol_string(&protocol_len, header->protocol);

  len = method_len + header->request_uri_len + protocol_len + 3;

  for (size_t i = 0; i < header->fields_len; i++) {
    size_t field_len;
    const char* field_str =
      dh_header_field_string(&field_len, &header->fields[i]);
    len += field_len + 2;
    free((void*)field_str);
  }

  char* str = malloc(len);
  CHECK_MALLOC(str);

  snprintf(str, len, "%s %s %s", method_str, header->request_uri, protocol_str);
  for (size_t i = 0; i < header->fields_len; i++) {
    size_t field_len;
    const char* field_str =
      dh_header_field_string(&field_len, &header->fields[i]);
    strncat(str, "\n", 1);
    strncat(str, field_str, len - strlen(str) - 1);
    free((void*)field_str);
  }

  *length = len;
  return str;
}

const char* dh_message_string(dh_message* message) {
  size_t header_len;
  const char* header_str = dh_header_string(&header_len, message->header);
  size_t len = header_len + message->body_len + 1;
  char* str = malloc(len);
  CHECK_MALLOC(str);

  snprintf(str, len, "%s\n%s", header_str, message->body);
  free((void*)header_str);
  return str;
}
