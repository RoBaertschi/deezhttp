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

void parse_until_content_length(int cfd) {}

void handle_message(int cfd) {
  /*char status_line[] = "HTTP/1.0 200 OK\r\nContent-Type: text/plain\r\n\r\n";*/
  /*write(cfd, status_line, sizeof(status_line) / sizeof(char) - 1);*/
  /*write(cfd, message, message_len);*/
}

int main(int argc, char** argv) {
  int sockfd, len;
  struct sockaddr_in addr;
  socklen_t addr_size;
  bool newline_found = false;
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
        char* new_string = malloc(nbytes_read + string_size * sizeof(char));
        memcpy(new_string, string, string_size);
        free(string);
        string = new_string;
        string_cap = nbytes_read + string_size;
      }

      memcpy(string + string_size, buffer, nbytes_read);
      string_size += nbytes_read;

      if (!received) {
        printf("received:\n");
        received = true;
      }
      write(STDOUT_FILENO, buffer, nbytes_read);
      if (buffer[nbytes_read - 1] == '\n') {
        newline_found = true;
      }
      for (int i = 0; i < nbytes_read; i++) {
        buffer[i]++;
      }
      if (newline_found) {
        printf("\n");
        newline_found = false;
        break;
      }
    }
    if (string_cap <= string_size) {
      string = realloc(string, string_size + 1);
      string[string_size] = '\0';
    }
    string_size += 1;
    /*handle_message(cfd, string, string_size);*/
    received = false;
    free(string);
    string_size = 0;
    string_cap = 0;
    string = NULL;
    close(cfd);
    printf("closed connection with client\n");
  }

  return 0;
}
