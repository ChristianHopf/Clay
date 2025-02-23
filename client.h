#ifndef CLIENT_H
#define CLIENT_H

#include <sys/socket.h>
#include <netinet/in.h>
#include <poll.h>
#include <unistd.h>

/* Connects to an IRC server
 * Returns socket file descriptor (-1 on error)
 */
int connect_to_server(const char *ip);

/* Sends an IRC command via the socket sockfd
 * command: string representing command type (ex. NICK, USER, etc)
 * text: text of the command (ex. text after 'USER' in 'USER bruh * * :Bro Brah')
 */
int send_command(int sockfd, const char *command, const char *text);

/* Polls for data in the socket sockfd via poll(),
 * then reads data into buffer
 */
int poll_and_read(int sokcfd, const char *buffer, size_t buf_size);

#endif
