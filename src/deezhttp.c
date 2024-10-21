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

  dh_message message = {
      .header =
          &(dh_header){.method = DH_GET,
                       .request_uri = "/",
                       .protocol = DH_HTTP_1_0,
                       .fields = (dh_header_field[]){{.key = "Host",
                                                      .value = "localhost"}},
                       .fields_len = 1},
      .body = "Hello, world!",
      .body_len = 13};

  const char *message_str = dh_message_string(&message);
  if (message_str == NULL) {
    return 1;
  }
  printf("%s\n", message_str);
  free((void *)message_str);
  return 0;
}
