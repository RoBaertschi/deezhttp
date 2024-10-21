#ifndef DEEZHTTP_HEADER_H_
#define DEEZHTTP_HEADER_H_

#include <stddef.h>

typedef enum { DH_GET, DH_POST, DH_HEAD } dh_http_method;
const char* dh_header_method_string(size_t* length, dh_http_method method);

typedef enum { DH_HTTP_1_0, DH_HTTP_INVALID } dh_protocol;
const char* dh_header_protocol_string(size_t* length, dh_protocol protocol);

typedef enum { DH_OK, DH_BAD_REQUEST, DH_NOT_FOUND } dh_status_code;
const char* dh_status_code_string(size_t* length, dh_status_code status_code);

typedef struct {
  const char* key;
  const char* value;
  size_t key_len;
  size_t value_len;
} dh_header_field;

const char* dh_header_field_string(size_t* length, dh_header_field* field);

typedef struct {
  dh_header_field* fields;
  size_t fields_len;
} dh_header;

const char* dh_header_string(size_t* length, dh_header* header);

typedef struct {
  dh_http_method method;
  const char* request_uri;
  size_t request_uri_len;
  dh_protocol protocol;
  dh_header* header;
  const char* body;
  size_t body_len;
} dh_request;

const char* dh_request_string(size_t* length, dh_request* request);

typedef struct {
  dh_status_code status_code;
  dh_header* header;
  const char* body;
  size_t body_len;
} dh_response;

const char* dh_response_string(size_t* length, dh_response* response);

#endif  // DEEZHTTP_HEADER_H_
