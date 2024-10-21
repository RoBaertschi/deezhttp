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
  size_t len = 0;
  for (size_t i = 0; i < header->fields_len; i++) {
    len += header->fields[i].key_len + header->fields[i].value_len + 4;
  }

  char* str = malloc(len);
  CHECK_MALLOC(str);

  size_t offset = 0;
  for (size_t i = 0; i < header->fields_len; i++) {
    dh_header_field* field = &header->fields[i];
    size_t field_len;
    const char* field_str = dh_header_field_string(&field_len, field);
    memcpy(str + offset, field_str, field_len);
    offset += field_len;
    free((void*)field_str);
  }

  *length = len;
  return str;
}
const char* dh_request_string(size_t* length, dh_request* request) {
  size_t len = 0;
  size_t method_len = 0;
  size_t protocol_len = 0;

  const char* method_str =
    dh_header_method_string(&method_len, request->method);
  const char* protocol_str =
    dh_header_protocol_string(&protocol_len, request->protocol);

  len += method_len + request->request_uri_len + protocol_len + 4;

  for (size_t i = 0; i < request->header->fields_len; i++) {
    len += request->header->fields[i].key_len +
           request->header->fields[i].value_len + 4;
  }

  len += request->body_len;
  char* str = malloc(len + 1);
  CHECK_MALLOC(str);

  size_t offset = 0;
  memcpy(str + offset, method_str, method_len);
  offset += method_len;

  str[offset] = ' ';
  offset += 1;

  memcpy(str + offset, request->request_uri, request->request_uri_len);
  offset += request->request_uri_len;
  str[offset] = ' ';
  offset += 1;

  memcpy(str + offset, protocol_str, protocol_len);
  offset += protocol_len;
  str[offset] = '\n';
  offset += 1;

  for (size_t i = 0; i < request->header->fields_len; i++) {
    dh_header_field* field = &request->header->fields[i];
    size_t field_len;
    const char* field_str = dh_header_field_string(&field_len, field);
    memcpy(str + offset, field_str, field_len);
    offset += field_len;
    free((void*)field_str);
  }

  if (request->body_len > 0) {
    memcpy(str + offset, request->body, request->body_len);
    offset += request->body_len;
  }

  str[offset] = '\0';
  *length = offset;

  return str;
}
