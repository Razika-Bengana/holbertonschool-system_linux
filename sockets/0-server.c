#include "socket.h"

/**
 * main - the entry point
 *
 * this program initializes and starts a TCP server on port 12345;
 * it creates a socket, sets options for reusing the address and port,
 * configures the server address structure, binds the socket to the specified
 * address and port, starts listening for incoming connections, and enters
 * an infinite loop to maintain the server listening state
 *
 * Return: 0 (success)
 */

int main(void)
{
	int server_fd, opt = 1;
	struct sockaddr_in address;

	server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (server_fd == 0)
	{
		perror("socket failed");
		exit(EXIT_FAILURE);
	}
	/* Configuration du socket pour réutiliser l'adresse et le port */
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
		       &opt, sizeof(opt)))
	{
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}
	/* Configuration de la structure d'adresse pour le serveur */
	address.sin_family = AF_INET; /* Famille d'adresses IPv4 */
	address.sin_addr.s_addr = INADDR_ANY;
	/* Accepte les connexions de toutes les interfaces réseau */
	address.sin_port = htons(12345);
	/* Port sur lequel le serveur écoutera les connexions */

	if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
	{
		perror("bind failed");
		exit(EXIT_FAILURE);
	}
	if (listen(server_fd, 3) < 0)
	{
		perror("listen");
		exit(EXIT_FAILURE);
	}
	printf("Server listening on port 12345...\n");

	while (1)
	{
		sleep(60); /* Pause pendant 60 secondes avant de vérifier à nouveau */
	}
	return (0);
}
