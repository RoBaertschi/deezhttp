#include "parser.h"
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include "header.h"

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
