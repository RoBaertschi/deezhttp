#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdbool.h>
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

void handle_message(const char* message, size_t message_len) {}

int main(int argc, char** argv) {
  dh_response response = {
    .protocol = DH_HTTP_1_0,
    .status_code = DH_OK,
    .header = NULL,
    .body = "Hello, World!",
    .body_len = 13,
  };

  size_t response_len;
  const char* response_str = dh_response_string(&response_len, &response);
  printf("%s\n", response_str);
  free(response_str);

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
    char* string = NULL;
    size_t string_cap = 0;
    size_t string_size = 0;
    bool received = false;
    while ((nbytes_read = read(cfd, buffer, BUFSIZ)) > 0) {
      if (nbytes_read + string_size > string_cap) {
        string = malloc(nbytes_read + string_size * sizeof(char));
        string_cap = nbytes_read + string_size;
      }

      if (!received) {
        printf("received:\n");
        received = true;
      }
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
    received = false;
    free(string);
    close(cfd);
  }

  return 0;
}
