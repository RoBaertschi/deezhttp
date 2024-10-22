#include "parser.h"

#include <SDL_net.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "buffer.h"
#include "header.h"

typedef struct {
  size_t pos;
  size_t peek_pos;
  char ch;
  dh_buffer *buffer;
  dh_request wip;
} parser;

void parser_read_ch(parser *p) {
  if (p->peek_pos >= p->buffer->size) {
    p->ch = 0;
  } else {
    p->ch = p->buffer->buffer[p->peek_pos];
  }
  p->pos = p->peek_pos;
  p->peek_pos += 1;
}

bool parser_has_char(parser *p, size_t pos) {
  if (pos >= p->buffer->size) {
    return false;
  }
  return true;
}

void parser_eat_space(parser *p) {
  while (p->ch == ' ' || p->ch == '\t') {
    parser_read_ch(p);
  }
}

static const char *methods[] = {
#define X(name, enum) name,
  DH_HTTP_METHODS
#undef X
};
static const size_t methods_lengths[] = {
#define X(name, enum) sizeof(name) / sizeof(char),
  DH_HTTP_METHODS
#undef X
};
static const dh_http_method methods_map[] = {
#define X(name, enum) enum,
  DH_HTTP_METHODS
#undef X
};

static const size_t methods_len = sizeof(methods) / sizeof(char *);

dh_http_method parse_method(parser *p) {
  for (size_t i = 0; i < methods_len; i++) {
    if (p->ch == methods[i][0]) {
      for (size_t j = 0; j < methods_lengths[i] - 1; j++) {
        if (!parser_has_char(p, p->pos + j)) {
         break;
        }
        if (p->buffer->buffer[p->pos + j] != methods[i][j]) {
          break;
        }

        if (j + 1 < methods_lengths[i] - 1) {
          for (size_t k = 0; k < methods_lengths[i] - 1; k++) {
            parser_read_ch(p);
          }
          return methods_map[i];
        }
      }
    }
  }

  return DH_METHOD_INVALID;
}


rdh_request dh_parse_request(dh_buffer *buffer) {
  parser p = {
    .buffer = buffer,
    .wip = {0},
    .pos = 0,
    .ch = 0,
  };
  if (0 < buffer->size) {
    p.ch = buffer->buffer[0];
  }

  dh_http_method method = parse_method(&p);
  if (method == DH_METHOD_INVALID) {
   return rdh_request_err(DH_PARSE_INVALID_METHOD);
  }

  size_t null = 0;
  printf("%s", dh_header_method_string(&null, method));

  parser_eat_space(&p);

  return rdh_request_ok(p.wip);
}

dh_http_method get_method(int cfd, bool *space_eaten) {
  *space_eaten = false;
  char buffer[4] = {0};
  size_t read_bytes = read(cfd, buffer, sizeof(buffer));
  if (read_bytes < 4) {
    fprintf(stderr, "the http request is to short\n");
    return DH_METHOD_INVALID;
  }

  if (strncmp(buffer, "GET ", 4) == 0) {
    *space_eaten = true;
    return DH_GET;
  } else if (strncmp(buffer, "POST", 4) == 0) {
    return DH_POST;
  } else if (strncmp(buffer, "HEAD", 4) == 0) {
    return DH_HEAD;
  }
  fprintf(stderr, "the http request has an invalid request method\n");
  return DH_METHOD_INVALID;
}

char eat_spaces(int cfd) {
  char c = 0;
  size_t bytes = 0;
  do {
    bytes = read(cfd, &c, 1);
  } while (c == ' ' && bytes != 0);
  return c;
}

dh_request parse_request(int cfd, bool *invalid) {
  bool space_eaten = false;
  dh_http_method method = get_method(cfd, &space_eaten);
  if (method == DH_METHOD_INVALID) {
    *invalid = true;
    return (dh_request){0};
  }

  if (!space_eaten) {
    char space = 0;
    size_t bytes = read(cfd, &space, 1);
    if (bytes != 1) {
      fprintf(stderr, "could not read space\n");
      *invalid = true;
      return (dh_request){0};
    }
  }
  char c = eat_spaces(cfd);
}
