#include <iostream>

#include <SDL2/SDL.h>
#include <SDL_net.h>


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv) {
  TCPsocket sd, csd;
  /* Socket descriptor, Client socket descriptor */
  IPaddress ip, *remoteIP;
  std::cout<< "Host: " <<ip.host<<std::endl;
  std::cout<< "Port: " <<ip.port<<std::endl;
  int quit, quit2;
  char buffer[512];

  if (SDLNet_Init() < 0) {
    fprintf(stderr, "SDLNet_Init: %s\n", SDLNet_GetError());
    exit(EXIT_FAILURE);
  }

  /* Resolving the host using NULL make network interface to listen */
  if (SDLNet_ResolveHost(&ip,  , 2000) < 0) {
    fprintf(stderr, "SDLNet_ResolveHost: %s\n", SDLNet_GetError());
    exit(EXIT_FAILURE);
  }

  std::cout<< "Host: " <<ip.host<<std::endl;
  std::cout<< "Port: " <<ip.port<<std::endl;


  /* Open a connection with the IP provided (listen on the host's port) */
  if (!(sd = SDLNet_TCP_Open(&ip))) {
    fprintf(stderr, "SDLNet_TCP_Open: %s\n", SDLNet_GetError());
    exit(EXIT_FAILURE);
  }

  /* Wait for a connection, send data and term */
  quit = 0;
  while (!quit) {
    std::cout<<"Waiting for connection..." << std::endl;
    /* This check the sd if there is a pending connection. * If there is one, accept that, and open a new socket for communicating */
    if ((csd = SDLNet_TCP_Accept(sd))) {
      /* Now we can communicate with the client using csd socket * sd will remain opened waiting other connections */

      /* Get the remote address */
      if ((remoteIP = SDLNet_TCP_GetPeerAddress(csd))) {
        /* Print the address, converting in the host format */
        printf("Host connected: %x %d\n", SDLNet_Read32(&remoteIP->host), SDLNet_Read16(&remoteIP->port));
      }
      else {
        fprintf(stderr, "SDLNet_TCP_GetPeerAddress: %s\n", SDLNet_GetError());
      }

      quit2 = 0;
      while (!quit2) {
        if (SDLNet_TCP_Recv(csd, buffer, 512) > 0) {
          printf("Client say: %s\n", buffer);

          /* Terminate this connection */
          if(strcmp(buffer, "exit") == 0) {
            quit2 = 1;
            printf("Terminate connection\n");
          }

          /* Quit the program */
          if(strcmp(buffer, "quit") == 0){
            quit2 = 1;
            quit = 1;
            printf("Quit program\n");
          }
        }
      }
  /* Close the client socket */
  SDLNet_TCP_Close(csd);
    }
  }

  SDLNet_TCP_Close(sd); SDLNet_Quit();

  return EXIT_SUCCESS;
}
