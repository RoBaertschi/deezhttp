#ifndef DEEZHTTP_PARSER_H_
#define DEEZHTTP_PARSER_H_

#include "header.h"
#include "src/buffer.h"
#include <stdbool.h>
#include "result.h"

#include <stdio.h>

RESULT(rdh_request, dh_request, bool)


dh_request dh_parse_request(dh_buffer *buffer);

#endif // DEEZHTTP_PARSER_H_
