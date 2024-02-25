#include "socket.h"

#define PORT 8080
#define BUFFER_SIZE 1024

void handle_connection(int client_socket)
{
	char buffer[BUFFER_SIZE];
	int bytes_received;

	struct sockaddr_in client_addr;
	socklen_t client_addr_len = sizeof(client_addr);
	getpeername(client_socket, (struct sockaddr*)&client_addr, &client_addr_len);
	printf("Connection accepted from %s\n", inet_ntoa(client_addr.sin_addr));

	bytes_received = recv(client_socket, buffer, BUFFER_SIZE - 1, 0);
	if (bytes_received < 0)
	{
		perror("recv");
		exit(EXIT_FAILURE);
	}

	buffer[bytes_received] = '\0';
	printf("Received HTTP request:\n%s\n", buffer);

	char method[10], path[100], version[10];
	sscanf(buffer, "%s %s %s", method, path, version);
	printf("Method: %s, Path: %s, Version: %s\n", method, path, version);

	char *response = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\nHello, World!";
	send(client_socket, response, strlen(response), 0);

	close(client_socket);
}

int main(void)
{
	int server_socket, client_socket;
	struct sockaddr_in server_addr;

	server_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (server_socket < 0)
	{
		perror("socket");
		exit(EXIT_FAILURE);
	}

	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(PORT);

	if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0)
	{
		perror("bind");
		exit(EXIT_FAILURE);
	}

	if (listen(server_socket, 10) < 0)
	{
		perror("listen");
		exit(EXIT_FAILURE);
	}

	printf("Server is listening on port %d...\n", PORT);

	while (1)
	{
		client_socket = accept(server_socket, NULL, NULL);
		if (client_socket < 0)
		{
			perror("accept");
			continue;
		}

		handle_connection(client_socket);
	}


	close(server_socket);
	return 0;
}
