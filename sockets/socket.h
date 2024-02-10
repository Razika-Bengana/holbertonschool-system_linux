#ifndef SOCKET_H
#define SOCKET_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

/* task 0 */
int create_server_socket(void);
void configure_server_socket(int server_fd);
void initialize_address(struct sockaddr_in *address);
void bind_server_socket(int server_fd, struct sockaddr_in *address);
void start_listening(int server_fd);


/* task 1 */
void initialize_address(struct sockaddr_in *address);
int create_and_setup_server_socket(struct sockaddr_in *address);


#endif /* SOCKET_H */
