#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>

// #define PORT "6667"

int main() {
  // Create socket
  int sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock < 0){
    perror("Socket creation failed");
    exit(1);
  }

  // Resolve hostname
  struct addrinfo hints = {0};
  struct addrinfo *res;
  int status;
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  if ((status = getaddrinfo("irc.libera.chat", "6667", &hints, &res) != 0)) {
    fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
    close(sock);
    exit(1);
  }

  // Define server info and connect via the socket
  // struct sockaddr_in server = {0};
  // server.sin_family = AF_INET;
  // server.sin_port = htons(6667);
  // inet_pton(AF_INET, "149.56.96.213", &server.sin_addr);

  // Connect to the server via the socket, using the resolved addrinfo
  if (connect(sock, res->ai_addr, res->ai_addrlen) < 0){
    perror("Connection failed");
    freeaddrinfo(res);
    close(sock);
    exit(1);
  }
  printf("Connection success!\n");
  freeaddrinfo(res);
 
  // Send IRC registration commands through the socket
  char *nick = "NICK test\r\n";
  char *user = "USER test 0 * :Test User\r\n";
  send(sock, nick, strlen(nick), 0);
  send(sock, user, strlen(user), 0);

  // Loop recv into a 512-character buffer (max IRC msg length)
  char buffer[512];
  while (recv(sock, buffer, sizeof(buffer) - 1, 0) > 0) {
    buffer[511] = '\0';
    printf("%s", buffer);
    // If received message starts with PING, respond with "PONG <token sent by server>"
    if (strstr(buffer, "PING") == buffer) {
      char pong[512];
      snprintf(pong, sizeof(pong), "PONG %s\r\n", buffer + 5);
      send(sock, pong, strlen(pong), 0);
    }
    // Clear buffer before receiving next message
    memset(buffer, 0, sizeof(buffer));
  }

  // CLose the socket before exiting
  close(sock);
  return 0;
}
