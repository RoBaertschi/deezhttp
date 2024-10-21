#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <unistd.h>

#include "error.h"
#include "src/header.h"

#define PROJECT_NAME "deezhttp"

int main(int argc, char** argv) {
  dh_request request = (dh_request){
    .method = DH_GET,
    .request_uri = "/",
    .request_uri_len = 1,
    .protocol = DH_HTTP_1_0,
    .header =
      &(dh_header){
        .fields =
          (dh_header_field[]){
            (dh_header_field){.key = "Host",
                              .value = "localhost",
                              .key_len = 5,
                              .value_len = 10},
            (dh_header_field){.key = "User-Agent",
                              .value = "deezhttp",
                              .key_len = 11,
                              .value_len = 9},
            (dh_header_field){
              .key = "Accept", .value = "*/*", .key_len = 6, .value_len = 3},
          },
        .fields_len = 3,
      },
    .body = "",
    .body_len = 0,
  };

  size_t request_str_len = 0;
  const char* request_str = dh_request_string(&request_str_len, &request);
  printf("%s\n", request_str);
  free((void*)request_str);

  int sockfd, len;
  struct sockaddr_in addr;
  socklen_t addr_size;
  int newline_found = 0;
  char protoname[] = "tcp";

  struct protoent* protoent;
  protoent = getprotobyname(protoname);

  if (protoent == NULL) {
    panic("Could not get protoent entry for tcp %s", strerror(errno));
  }

  int sfd = socket(AF_INET, SOCK_STREAM, protoent->p_proto);

  if (sfd == -1) {
    panic("Could not create socket %s", strerror(errno));
  }
  int enable = 1;

  if (setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(enable)) < 0) {
    panic("setsockopt(SO_REUSEADDR) failed %s", strerror(errno));
  }

  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = htonl(INADDR_ANY);
  addr.sin_port = htons(8086);
  if (bind(sfd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
    panic("Could not bind port 8086 %s", strerror(errno));
  }

  if (listen(sfd, 5) == -1) {
    panic("Failed to listen to port %s", strerror(errno));
  }

  fprintf(stderr, "listening on port 8086\n");

  while (1) {
    struct sockaddr_in client_addr = {0};
    socklen_t client_len = sizeof(client_addr);
    int cfd = accept(sfd, (struct sockaddr*)&client_addr, &client_len);

    int nbytes_read = 0;
    char buffer[BUFSIZ] = {0};
    while ((nbytes_read = read(cfd, buffer, BUFSIZ)) > 0) {
      printf("received:\n");
      write(STDOUT_FILENO, buffer, nbytes_read);
      if (buffer[nbytes_read - 1] == '\n') {
        newline_found = 1;
      }
      for (int i = 0; i < nbytes_read; i++) {
        buffer[i]++;
      }
      write(cfd, buffer, nbytes_read);
      if (newline_found) {
        break;
      }
    }
    close(cfd);
  }

  return 0;
}
