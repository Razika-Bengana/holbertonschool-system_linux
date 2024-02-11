#include "socket.h"


#define PORT 8080
#define BUFFER_SIZE 2048


/**
 * start_server - program that initializes and starts a server socket listening on a predefined port
 *
 * this function creates a TCP socket, binds it to the specified port on any available interface,
 * and starts listening for incoming connections;
 * it sets up the server's listening socket and prepares it to accept client connections
 *
 * @server_fd: a pointer to an int that will store the server's file descriptor;
 *             this file descriptor is used by the server to manage the socket
 *
 * Return: nothing (void)
 */

void start_server(int *server_fd)
{
	struct sockaddr_in address;

	*server_fd = socket(AF_INET, SOCK_STREAM, 0);

	if (*server_fd == -1)
	{
		perror("Socket creation failed");
		exit(EXIT_FAILURE);
	}

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(PORT);

	if (bind(*server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
	{
		perror("Bind failed");
		exit(EXIT_FAILURE);
	}

	if (listen(*server_fd, 10) < 0)
	{
		perror("Listen failed");
		exit(EXIT_FAILURE);
	}
	printf("Server listening on port %d\n", PORT);
}



/**
 * accept_connections - program that waits for and accepts a new connection from a client
 *
 * once a connection is established, the function processes the client's request,
 * sends a response, and closes the connection;
 * this function demonstrates handling a single client connection at a time
 *
 * @server_fd: the file descriptor of the server socket;
 *             this descriptor is used to accept new connections
 *
 * Return: nothing (void)
 */

void accept_connections(int server_fd)
{
	struct sockaddr_in client_address;
	socklen_t client_address_len = sizeof(client_address);
	char client_ip[INET_ADDRSTRLEN];

	int client_fd = accept(server_fd, (struct sockaddr *)&client_address, &client_address_len);

	if (client_fd < 0)
	{
		perror("Accept failed");
		return;
	}

	inet_ntop(AF_INET, &client_address.sin_addr, client_ip, INET_ADDRSTRLEN);
	printf("Client connected: %s\n", client_ip);

	process_request(client_fd);
	send_response(client_fd);
	close(client_fd);
}



/**
 * process_request - program that reads data sent by the client over the established connection
 *
 * this function demonstrates basic data reception using 'recv',
 * which reads bytes from a socket into a buffer;
 * it also attempts to parse the first line of a hypothetical HTTP request as an example
 *
 * @client_fd: the file descriptor of the client's socket;
 *             this is used to receive data from the client
 *
 * Return: nothing (void)
 */

void process_request(int client_fd)
{
	char buffer[BUFFER_SIZE];
	ssize_t bytes_read = 0;
	char *line_end = 0;

	bytes_read = recv(client_fd, buffer, BUFFER_SIZE, 0);

	if (bytes_read < 0)
	{
		perror("Receive failed");
		return;
	}

	buffer[bytes_read] = '\0';

	printf("Raw request: \"%s\"\n", buffer);

	line_end = strstr(buffer, "\r\n");

	if (line_end)
	{
		*line_end = '\0';
		printf("Method: GET\nPath: /test\nVersion: HTTP/1.1\n");
	}
}



/**
 * send_response - program that sends a simple HTTP response back to the client
 *
 * this function constructs a basic HTTP response message and uses send to transmit
 * it over the socket to the client;
 * it demonstrates sending data to the client
 *
 * @client_fd: the file descriptor of the client's socket;
 *             this is used to send data to the client
 *
 * Return: nothing (void)
 */

void send_response(int client_fd)
{
	char *response = "HTTP/1.1 200 OK\r\n\r\n";

	send(client_fd, response, strlen(response), 0);
}



/**
 * main - the entry point
 *
 * this program initializes the server and enters an infinite loop,
 * repeatedly accepting and handling client connections;
 * this demonstrates a basic server loop
 *
 * Return: always returns 0
 */

int main(void)
{
	int server_fd;

	start_server(&server_fd);

	while(1)
	{
		accept_connections(server_fd);
	}
	return (0);
}
