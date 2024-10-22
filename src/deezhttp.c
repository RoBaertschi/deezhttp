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
#include "buffer.h"

void parse_until_content_length(int cfd) {}

void handle_message(int cfd, dh_buffer* stream) {
  // Example: Send a simple HTTP response
  char status_line[] = "HTTP/1.0 200 OK\r\nContent-Type: text/plain\r\n\r\n";
  write(cfd, status_line, sizeof(status_line) - 1);
  write(cfd, stream->buffer, stream->size);
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

    // Initialize a new stream
    dh_buffer stream = dh_stream_new(0);

    int nbytes_read = 0;
    char buffer[BUFSIZ] = {0};
    bool received = false;

    // Read data into the stream
    while ((nbytes_read = read(cfd, buffer, BUFSIZ)) > 0) {
      // Push the data into the stream
      dh_stream_push(&stream, buffer, nbytes_read);

      if (!received) {
        printf("received:\n");
        received = true;
      }
      write(STDOUT_FILENO, buffer, nbytes_read);

      // Check for a newline to terminate the reading loop
      if (buffer[nbytes_read - 1] == '\n') {
        newline_found = true;
      }
      if (newline_found) {
        printf("\n");
        newline_found = false;
        break;
      }
    }

    // Null-terminate the stream buffer for easier handling of string data
    dh_stream_push(&stream, "\0", 1);
    printf("stream size: %zu\n", stream.size);

    // Handle the received message
    handle_message(cfd, &stream);

    // Clean up and free the stream buffer
    received = false;
    dh_stream_shrink(&stream);  // Shrink the stream before ending
    free(stream.buffer);
    close(cfd);
    printf("closed connection with client\n");
  }

  return 0;
}
