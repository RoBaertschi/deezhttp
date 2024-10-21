#include "header.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
      *length = 9;
      return "HTTP/1.0";
    case DH_HTTP_INVALID:
      *length = 8;
      return "INVALID";
  }

  *length = 0;
  return 0;
}

const char* dh_header_method_string(size_t* length, dh_http_method method) {
  if (method == DH_GET) {
    *length = 4;
    return "GET";
  } else if (method == DH_POST) {
    *length = 5;
    return "POST";
  } else if (method == DH_HEAD) {
    *length = 5;
    return "HEAD";
  }

  *length = 0;
  return 0;
}

const char* dh_status_code_string(size_t* length, dh_status_code status_code) {
  switch (status_code) {
    case DH_OK:
      *length = 3;
      return "OK";
    case DH_BAD_REQUEST:
      *length = 12;
      return "BAD REQUEST";
    case DH_NOT_FOUND:
      *length = 10;
      return "NOT FOUND";
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
  size_t len, method_len, protocol_len;

  const char* method_str =
    dh_header_method_string(&method_len, request->method);
  const char* protocol_str =
    dh_header_protocol_string(&protocol_len, request->protocol);

  size_t header_len;
  const char* header_str = dh_header_string(&header_len, request->header);

  len = method_len + request->request_uri_len + protocol_len + header_len +
        request->body_len + 6;

  char* str = malloc(len);
  CHECK_MALLOC(str);

  snprintf(str, len, "%s %s %s\n%s\n%s", method_str, request->request_uri,
           protocol_str, header_str, request->body);

  *length = len;
  return str;
}

const char* dh_response_string(size_t* length, dh_response* response) {
  size_t len, status_code_len;

  const char* status_code_str =
    dh_status_code_string(&status_code_len, response->status_code);

  size_t header_len;
  const char* header_str = dh_header_string(&header_len, response->header);

  len = status_code_len + header_len + response->body_len + 4;

  char* str = malloc(len);
  CHECK_MALLOC(str);

  snprintf(str, len, "%s\n%s\n%s", status_code_str, header_str, response->body);

  *length = len;
  return str;
}
