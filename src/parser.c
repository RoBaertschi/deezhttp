#include "parser.h"

#include <SDL_net.h>
#include <stddef.h>
#include <stdio.h>
#include <unistd.h>

#include "buffer.h"
#include "header.h"
#include "utils.h"

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
      bool match = true;
      for (size_t j = 0; j < methods_lengths[i] - 1; j++) {
        if (!parser_has_char(p, p->pos + j) ||
            p->buffer->buffer[p->pos + j] != methods[i][j]) {
          match = false;
          break;
        }
      }
      if (match) {
        for (size_t k = 0; k < methods_lengths[i]; k++) {
          parser_read_ch(p);
        }
        return methods_map[i];
      }
    }
  }
  return DH_METHOD_INVALID;
}

rdh_uri dh_parse_uri(parser *p) {
  size_t start = p->pos;
  if (p->ch == ' ') {
    parser_eat_space(p);
  }
  while (p->ch != ' ' && p->ch != '\t') {
    parser_read_ch(p);
  }
  size_t end = p->pos;
  size_t len = end - start;
  char *uri = malloc(len + 1);
  if (uri == NULL) {
    return rdh_uri_err(DH_PARSE_INVALID_METHOD);
  }
  memcpy(uri, p->buffer->buffer + start, len);
  uri[len] = '\0';
  return rdh_uri_ok(uri);
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

  rdh_uri uri = dh_parse_uri(&p);
  if (rdh_uri_is_err(uri)) {
    return rdh_request_err(DH_PARSE_INVALID_METHOD);
  }

  size_t null = 0;
  printf("%s\n", dh_header_method_string(&null, method));
  printf("%s\n", rdh_uri_unwrap(uri));

  parser_eat_space(&p);

  return rdh_request_ok(p.wip);
}
