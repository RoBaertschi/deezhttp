#include <SDL2/SDL.h>
#include <SDL2/SDL_error.h>
#include <SDL_net.h>
#include <stdbool.h>
#include <stdio.h>
#include <time.h>
#include "buffer.h"

void parse_until_content_length(int cfd) {}

void handle_message(int cfd) {
  /*char status_line[] = "HTTP/1.0 200 OK\r\nContent-Type: text/plain\r\n\r\n";*/
  /*write(cfd, status_line, sizeof(status_line) / sizeof(char) - 1);*/
  /*write(cfd, message, message_len);*/
}

int main(int argc, char** argv) {
  if (SDL_Init(0) != 0) {
    fprintf(stderr, "could not initalize SDL2 %s", SDL_GetError());
    return EXIT_FAILURE;
  }
  if (SDLNet_Init() < 0) {
    fprintf(stderr, "could not initalize SDL2Net %s", SDLNet_GetError());
    SDL_Quit();
    return EXIT_FAILURE;
  }

  IPaddress addr = {
    .host = INADDR_ANY,
    .port = 8086,
  };

  SDLNet_ResolveHost(&addr, NULL, 8086);
  printf("Server IP: %x, %d\n", addr.host, addr.port);
  TCPsocket socket = SDLNet_TCP_Open(&addr);
  SDLNet_SocketSet socketset = SDLNet_AllocSocketSet(1);
  if (socketset == NULL) {
    fprintf(stderr, "could not allocate socket set of size 1 %s", SDLNet_GetError());
    return EXIT_FAILURE;
  }

  SDLNet_AddSocket(socketset, (SDLNet_GenericSocket)socket);

  while (1) {
    SDLNet_CheckSockets(socketset, ~0);
    if (!SDLNet_SocketReady(socket)) {
      continue;
    }

    TCPsocket client = SDLNet_TCP_Accept(socket);
    if (client == NULL) {
      SDLNet_Quit();
      SDL_Quit();
      fprintf(stderr, "failed to accept tcp %s\n", SDLNet_GetError());
      continue;
    }

    int nbytes_read = 0;
    char buffer[BUFSIZ] = {0};
    bool newline_found = false;
    dh_buffer string_buffer = dh_buffer_new(0);

    while ((nbytes_read = SDLNet_TCP_Recv(client, buffer, BUFSIZ)) > 0) {
      dh_buffer_push(&string_buffer, buffer, nbytes_read);
      if (buffer[nbytes_read - 1] == '\n') {
        newline_found = true;
      }
      for (int i = 0; i < nbytes_read; i++) {
        buffer[i]++;
      }
      if (newline_found) {
        newline_found = false;
        break;
      }
    }
    printf("received:\n%s\n---\n", string_buffer.buffer);

    dh_buffer_push(&string_buffer, "\n", 2);
    /*handle_message(cfd, string, string_size);*/
    SDLNet_TCP_Close(client);
    printf("closed connection with client\n");
  }

  SDLNet_DelSocket(socketset, (SDLNet_GenericSocket)socket);
  SDLNet_TCP_Close(socket);

  SDLNet_Quit();
  SDL_Quit();


  return 0;
}
