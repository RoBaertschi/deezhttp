#include <SDL2/SDL.h>
#include <SDL2/SDL_error.h>
#include <SDL2/SDL_net.h>
#include <stdbool.h>
#include <stdio.h>
#include <time.h>

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
    char* string = NULL;
    size_t string_cap = 0;
    size_t string_size = 0;
    bool received = false;
    bool newline_found = false;

    while ((nbytes_read = SDLNet_TCP_Recv(client, buffer, BUFSIZ)) > 0) {
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
      fwrite(buffer, 1, nbytes_read, stdout);
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
    SDLNet_TCP_Close(client);
    printf("closed connection with client\n");
  }

  SDLNet_DelSocket(socketset, (SDLNet_GenericSocket)socket);
  SDLNet_TCP_Close(socket);

  SDLNet_Quit();
  SDL_Quit();


  return 0;
}
