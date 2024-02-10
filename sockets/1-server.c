#include "socket.h"

/**
 * main - the entry point
 *
 * this program creates an IPv4 TCP server listening on port 12345;
 * it initializes a server socket and binds it to the specified port
 * to listen for incoming connections;
 * upon a successful connection, it retrieves and prints the IP address
 * of the connected client;
 * the server then closes the client socket and terminates;
 * this server demonstrates basic socket programming concepts such as
 * socket creation, binding, listening for connections, and accepting
 * connections
 *
 * Return: 0 on successful execution,
 *         or exits with a failure status if any socket operation fails
 */

int main(void) {
	struct sockaddr_in address = {0};
	int server_fd, new_socket, opt = 1;
	char client_ip[INET_ADDRSTRLEN];

	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("socket failed");
		exit(EXIT_FAILURE);
	}
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(12345);

	if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
		perror("bind failed");
		exit(EXIT_FAILURE);
	}
	if (listen(server_fd, 3) < 0) {
		perror("listen");
		exit(EXIT_FAILURE);
	}
	printf("Server listening on port 12345\n");

	int addrlen = sizeof(address);
	if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) {
		perror("accept");
		exit(EXIT_FAILURE);
	}
	inet_ntop(AF_INET, &(address.sin_addr), client_ip, INET_ADDRSTRLEN);
	printf("Client connected: %s\n", client_ip);

	close(new_socket);
	close(server_fd);

	return (0);
}