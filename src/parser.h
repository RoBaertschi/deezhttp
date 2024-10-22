#ifndef DEEZHTTP_PARSER_H_
#define DEEZHTTP_PARSER_H_

#include "header.h"
#include <stdbool.h>

dh_request parse_request(int cfd, bool *invalid);

#endif // DEEZHTTP_PARSER_H_
