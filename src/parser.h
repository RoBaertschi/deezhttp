#ifndef DEEZHTTP_PARSER_H_
#define DEEZHTTP_PARSER_H_

#include <stdbool.h>

#include "header.h"
#include "result.h"
#include "src/buffer.h"

typedef enum { DH_PARSE_INVALID_METHOD } dh_parse_error;

#include <stdio.h>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-function"
RESULT(rdh_request, dh_request, dh_parse_error)
RESULT(rdh_uri, const char *, dh_parse_error)
#pragma GCC diagnostic pop

rdh_request dh_parse_request(dh_buffer *buffer);

#endif  // DEEZHTTP_PARSER_H_
