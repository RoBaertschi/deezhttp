#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "header.h"

#define PROJECT_NAME "deezhttp"

int main(int argc, char **argv) {
  if (argc != 1) {
    printf("%s takes no arguments.\n", argv[0]);
    return 1;
  }
  printf("This is project %s.\n", PROJECT_NAME);

  dh_header_field *fields = malloc(sizeof(dh_header_field) * 2);
  fields[0].key = "Host";
  fields[0].value = "localhost";
  fields[0].key_len = 4;
  fields[0].value_len = 9;
  fields[1].key = "User-Agent";
  fields[1].value = "deezhttp";
  fields[1].key_len = 10;
  fields[1].value_len = 7;

  dh_header header = {.method = DH_GET,
                      .request_uri = "/",
                      .request_uri_len = 1,
                      .protocol = DH_HTTP_1_0,
                      .fields = fields,
                      .fields_len = 2};

  size_t message_len;
  const char *message_str = dh_header_string(&message_len, &header);
  printf("%s\n", message_str);

  free((void *)message_str);
  free(fields);
  return 0;
}
