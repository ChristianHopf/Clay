#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>

#include "client.h"

// #define PORT "6667"

int connect_to_server(const char *ip){
  int sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock < 0){
    perror("Error: failed to create socket");
    return -1;
  }

  struct addrinfo hints = {0};
  struct addrinfo *res;
  int status;
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  if((status = getaddrinfo(ip, "6667", &hints, &res) != 0)) {
    fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
    close(sock);
    return -1;
  }

  // Connect to the server and return socket
  if (connect(sock, res->ai_addr, res->ai_addrlen) < 0){
    perror("Error: failed to connect to server");
    freeaddrinfo(res);
    close(sock);
    return -1;
  }

  freeaddrinfo(res);
  return sock;
}

int send_command(int sockfd, char *command, char *text){
  char *full_command = strcat(command, text);
  int full_length = strlen(full_command);
  send(sockfd, full_command, full_length, 0);
  return full_length;
}

//int main() {
//  // Loop recv into a 512-character buffer (max IRC msg length)
//  int sock;
//  char buffer[512];
//  while (recv(sock, buffer, sizeof(buffer) - 1, 0) > 0) {
//    buffer[511] = '\0';
//    printf("%s", buffer);
//    // If received message starts with PING, respond with "PONG <token sent by server>"
//    if (strstr(buffer, "PING") == buffer) {
//      char pong[512];
//      snprintf(pong, sizeof(pong), "PONG %s\r\n", buffer + 5);
//      send(sock, pong, strlen(pong), 0);
//    }
//    // Clear buffer before receiving next message
//    memset(buffer, 0, sizeof(buffer));
//  }
//
//  // CLose the socket before exiting
//  close(sock);
//  return 0;
//}
