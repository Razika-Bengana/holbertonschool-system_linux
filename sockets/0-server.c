#include "socket.h"


#define PORT 12345


/**
 * create_server_socket - program that creates a socket for the server to listen on
 *
 * this function sets up a stream socket using the IPv4 protocol;
 * if the socket cannot be created, the program prints an error message and exits
 *
 * Return: the file descriptor for the newly created socket
 */

int create_server_socket(void)
{
	int server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (server_fd == 0)
	{
		perror("socket failed");
		exit(EXIT_FAILURE);
	}
	return (server_fd);
}



/**
 * configure_server_socket - program that configures the server socket
 * with options to allow the reuse of local addresses and ports
 *
 * this function is useful for restarting a server that has bound sockets
 *
 * @server_fd: the file descriptor of the server socket to configure
 *
 * Return: nothing (void)
 */

void configure_server_socket(int server_fd)
{
	int opt = 1;
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
	{
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}
}



/**
 * initialize_address - program that initializes the 'sockaddr_in' structure
 * with settings for the server
 *
 * this function sets the family to IPv4, the address to listen on any interface,
 * and the port to a predefined value;
 * the port number is converted from host byte order to network byte order
 *
 * @address: a pointer to the 'sockaddr_in' structure to be initialized
 *
 * Return: nothing (void)
 */

void initialize_address(struct sockaddr_in *address)
{
	address->sin_family = AF_INET;
	address->sin_addr.s_addr = INADDR_ANY;
	address->sin_port = htons(PORT);
}



/**
 * bind_server_socket - program that binds the server socket to the specified
 * address and port
 *
 * this function associates the socket with its local address so it can listen
 * for incoming connections
 *
 * @server_fd: the file descriptor of the server socket to bind
 * @address: a pointer to the 'sockaddr_in' structure containing the address to bind to
 *
 * Return: nothing (void)
 */

void bind_server_socket(int server_fd, struct sockaddr_in *address)
{
	if (bind(server_fd, (struct sockaddr *)address, sizeof(*address)) < 0)
	{
		perror("bind failed");
		exit(EXIT_FAILURE);
	}
}



/**
 * start_listening - program that puts the server socket in a listening state,
 * allowing it to accept incoming connection requests
 *
 * this function can queue up to 3 connection requests before refusing new ones
 *
 * @server_fd: the file descriptor of the server socket to start listening on
 *
 * Return: nothing (void)
 */

void start_listening(int server_fd)
{
	if (listen(server_fd, 3) < 0)
	{
		perror("listen");
		exit(EXIT_FAILURE);
	}
	printf("Server listening on port %d...\n", PORT);
}



/**
 * main - the entry point
 *
 * this program orchestrates the creation, configuration, and listening of a server socket;
 * it initializes the address for the server, binds the server socket, and starts listening
 * for incoming connections;
 * the server runs indefinitely, pausing for 60 seconds in each iteration of the loop
 *
 * Return: 0 (success))
 */

int main(void)
{
	struct sockaddr_in address;

	int server_fd = create_server_socket();
	configure_server_socket(server_fd);
	initialize_address(&address);
	bind_server_socket(server_fd, &address);
	start_listening(server_fd);

	while (1)
	{
		sleep(60);
	}

	return (0);
}
