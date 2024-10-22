#ifndef DEEZHTTP_PARSER_H_
#define DEEZHTTP_PARSER_H_

#include "header.h"
#include "src/buffer.h"
#include <stdbool.h>
#include "result.h"

typedef enum {
  DH_PARSE_INVALID_METHOD
} dh_parse_error;

#include <stdio.h>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-function"
RESULT(rdh_request, dh_request, dh_parse_error)
#pragma GCC diagnostic pop


rdh_request dh_parse_request(dh_buffer *buffer);

#endif // DEEZHTTP_PARSER_H_
