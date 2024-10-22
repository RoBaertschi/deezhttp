#include "header.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "error.h"

const char* dh_header_field_string(size_t* length, dh_header_field* field) {
  size_t len = field->key_len + field->value_len + 4;
  char* str = malloc(len);
  CHECK_MALLOC(str);

  snprintf(str, len, "%s: %s\r\n", field->key, field->value);
  *length = len;
  return str;
}

inline const char* dh_header_protocol_string(size_t* length,
                                             dh_protocol protocol) {
  switch (protocol) {
    case DH_HTTP_1_0:
      *length = 9;
      return "HTTP/1.0";
    case DH_HTTP_0_9:
      *length = 9;
      return "HTTP/0.9";
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
  } else if (method == DH_METHOD_INVALID) {
    *length = 8;
    return "INVALID";
  }

  *length = 0;
  return 0;
}

const char* dh_header_string(size_t* length, dh_header* header) {
  if (header == NULL) {
    *length = 0;
    return "";
  }
  size_t len = 0;

  for (size_t i = 0; i < header->fields_len; i++) {
    len += header->fields[i].key_len + header->fields[i].value_len + 4;
  }

  char* str = malloc(len + 1);
  CHECK_MALLOC(str);

  str[0] = '\0';

  for (size_t i = 0; i < header->fields_len; i++) {
    size_t field_len;
    const char* field_str =
      dh_header_field_string(&field_len, &header->fields[i]);

    strcat(str, field_str);
    free((void*)field_str);
  }

  *length = len;
  return str;
}

const char* dh_header_status_reason(size_t* length,
                                    dh_status_code status_code) {
  switch (status_code) {
    case DH_OK:
      *length = 3;
      return "OK";
    case DH_CREATED:
      *length = 9;
      return "Created";
    case DH_ACCEPTED:
      *length = 10;
      return "Accepted";
    case DH_NO_CONTENT:
      *length = 11;
      return "No Content";
    case DH_MOVED_PERMANENTLY:
      *length = 18;
      return "Moved Permanently";
    case DH_MOVED_TEMPORARILY:
      *length = 18;
      return "Moved Temporarily";
    case DH_NOT_MODIFIED:
      *length = 13;
      return "Not Modified";
    case DH_BAD_REQUEST:
      *length = 12;
      return "Bad Request";
    case DH_UNAUTHORIZED:
      *length = 13;
      return "Unauthorized";
    case DH_FORBIDDEN:
      *length = 10;
      return "Forbidden";
    case DH_NOT_FOUND:
      *length = 10;
      return "Not Found";
    case DH_INTERNAL_SERVER_ERROR:
      *length = 22;
      return "Internal Server Error";
    case DH_NOT_IMPLEMENTED:
      *length = 16;
      return "Not Implemented";
    case DH_BAD_GATEWAY:
      *length = 12;
      return "Bad Gateway";
    case DH_SERVICE_UNAVAILABLE:
      *length = 20;
      return "Service Unavailable";
  }

  *length = 0;
  return 0;
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

  char* str = malloc(len + 1);
  CHECK_MALLOC(str);

  snprintf(str, len + 1, "%s %s %s\r\n%s\r\n%s", method_str,
           request->request_uri, protocol_str, header_str, request->body);

  *length = len;
  return str;
}

const char* dh_response_string(size_t* length, dh_response* response) {
  if (response->protocol == DH_HTTP_0_9) {
    *length = response->body_len;
    return response->body;
  }

  size_t len, protocol_len, header_len, reason_len;

  const char* protocol_str =
    dh_header_protocol_string(&protocol_len, response->protocol);

  const char* header_str = dh_header_string(&header_len, response->header);

  const char* reason_str =
    dh_header_status_reason(&reason_len, response->status_code);

  len = protocol_len + response->status_code + header_len + response->body_len +
        reason_len + 6;

  char* str = malloc(len + 1);
  CHECK_MALLOC(str);
  snprintf(str, len + 1, "%s %d %s\r\n%s\r\n%s", protocol_str,
           response->status_code, reason_str, header_str, response->body);

  *length = len;
  return str;
}
