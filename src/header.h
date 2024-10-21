#ifndef DEEZHTTP_HEADER_H_
#define DEEZHTTP_HEADER_H_

#include <stddef.h>

typedef enum { DH_GET, DH_POST, DH_HEAD } dh_http_method;
inline const char* dh_header_method_string(size_t* length,
                                           dh_http_method method) {
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
};

typedef enum { DH_HTTP_1_0, DH_HTTP_INVALID } dh_protocol;

inline const char* dh_header_protocol_string(dh_protocol protocol) {
  switch (protocol) {
    case DH_HTTP_1_0:
      return "HTTP/1.0";
    case DH_HTTP_INVALID:
      return "INVALID";
  }
};

typedef struct {
  const char* key;
  const char* value;
  size_t key_len;
  size_t value_len;
} dh_header_field;

const char* dh_header_field_string(dh_header_field* field);

typedef struct {
  dh_http_method method;
  const char* request_uri;
  dh_protocol protocol;
  dh_header_field* fields;
  size_t fields_len;
} dh_header;

const char* dh_header_string(dh_header* header);

typedef struct {
  dh_header* header;
  const char* body;
  size_t body_len;
} dh_message;

const char* dh_message_string(dh_message* message);

#endif  // DEEZHTTP_HEADER_H_