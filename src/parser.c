#include "parser.h"
#include <SDL_net.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include "buffer.h"
#include "header.h"

typedef struct {
  size_t pos;
  size_t peek_pos;
  char ch;
  dh_buffer *buffer;
  dh_request wip;
} parser;

void parser_read_ch(parser* p) {
  if (p->peek_pos >= p->buffer->size) {
    p->ch = 0;
  }
}

dh_request dh_parse_request(TCPsocket socket, dh_buffer *buffer) {
  parser parser = {
    .buffer = buffer,
    .wip = {0},
    .pos = 0,
    .ch = 0,
  };

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
  } while(c == ' ' && bytes != 0);
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
