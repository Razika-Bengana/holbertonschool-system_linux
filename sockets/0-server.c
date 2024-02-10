#include "socket.h"


#define PORT 12345


/**
 * setup_server_socket - program that prepares a server socket for communication
 *
 * this function performs several key steps to prepare the server to accept client
 * connections reliably:
 *
 * 1. creates a socket using the IPv4 protocol and stream communication
 * 2. sets socket options to allow address and port reuse, which is crucial
 *    for restarting a server without waiting for the port to be freed
 * 3. initializes the server's address structure to specify that the server
 *    should listen on all network interfaces using the defined PORT number
 * 4. binds the socket to the server's address
 * 5. sets the socket to listen for incoming connections, with a backlog of up
 *    to 3 pending connections
 *
 * @address: a pointer to the 'sockaddr_in' structure, which will be populated with
 *           the server's address settings
 *
 * Return: the file descriptor of the server socket that is ready to listen for connections
 */

int setup_server_socket(struct sockaddr_in *address)
{
	int server_fd = socket(AF_INET, SOCK_STREAM, 0);
	int opt = 1;

	if (server_fd == 0)
	{
		perror("socket failed");
		exit(EXIT_FAILURE);
	}

	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
	{
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}

	address->sin_family = AF_INET;
	address->sin_addr.s_addr = INADDR_ANY;
	address->sin_port = htons(PORT);

	if (bind(server_fd, (struct sockaddr *)address, sizeof(*address)) < 0)
	{
		perror("bind failed");
		exit(EXIT_FAILURE);
	}

	if (listen(server_fd, 3) < 0)
	{
		perror("listen");
		exit(EXIT_FAILURE);
	}

	printf("Server listening on port %d...\n", PORT);
	return server_fd;
}



/**
 *
 * @return
 */

int main(void)
{
	while (1)
	{
		sleep(60);
	}

	return 0;
}
