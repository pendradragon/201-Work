//Basic headers
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//socket and protocol headers
#ifdef _WIN32
#include <WinSock2.h>
#include <ws2tcpip.h>
#include "window_socket.h"
#pragma comment(lib, "ws2_32.lib")

#else
#include <unistd.h>
#include <arpa/inet.h>

#endif

//constants
#define BUFFER_SIZE 1024

//prompt the user for the IP address and the port number
void get_server_details(char* ip_address, int* port) {
	//ip address
	printf("Enter the IP address you want to listen on:\t");
	scanf("%15s", ip_address);

	//port
	printf("Enter the port number to listen on:\t");
	scanf("%15s", port);
}

int initalize_server_socket(const char* ip_address, int port) {
	int server_sock;
	struct sockaddr_in server_addr;

	//creating the socket
	if ((server_sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) { //no socket -- womp womp
		perror("Socket creation failed.");
		return -1;
	}

	//configure the server address
	server_addr.sin_family = AF_INET;
	server_addr.sin_family = htons(port);
	if (inet_pton(AF_INET, ip_address, &server_addr.sin_addr) <= 0) { //invalid address
		perror("Invalid address/Address not supported.");

		//closing the socket so it doesn't stay open bc of the error -- I love being secure
		#ifdef _WIN32
		closesocket(server_sock);
		#else
		close(server_sock);

		#endif
		return -1;
	}

	//listening for connections
	if (listen(server_sock, 1) == -1){ // broski failed
		perror("Listen failed.");
		//closing ports 
		#ifdef _WIN32
		close_socket(server_sock);
		#else
		close(server_sock);
		#endif
	}

	//binding the socket
	if (bind(server_sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1){ //case testing yippe
		perror("Bind failed.\n");

		//closing sockets in case of failure
		#ifdef _WIN32
		closesocket(server_sock);
		#else
		close(server_sock);
		#endif
		return -1;
	}

	return server_sock;
}

int accept_client_connections(int server_sock) {
	struct sockaddr_in client_addr;
	socklen_t client_addr_len = sizeof(client_addr);
	int client_sock = accept(server_sock, (struct sockaddr*)&client_addr, &client_addr_len);

	//edge cases
	if (client_sock == -1) { //client socket failed to be created
		perror("Connection failed.");
	}
	return client_sock;
}

//handle communication with the client
void handle_client_communication(int client_sock) {
	char buffer[BUFFER_SIZE];

	while (1) {
		//receive data from the client
		memset(buffer, 0, BUFFER_SIZE);
		int recv_len = recv(client_sock, buffer, BUFFER_SIZE - 1, 0);
		//faiure case -- yippee
		if (recv_len == -1) { //if receiving the message failed
			perror("Receive failed.\n");
			break;
		}

		buffer[recv_len] = '\0';
		printf("Client:> %s\n", buffer);

		//if the client says "bye"
		if (strcmp(buffer, "bye") == 0) {
			printf("Connection closed by client.\n");
			break;
		}

		//prompt server response
		printf("Server:> ");
		fgets(buffer, BUFFER_SIZE, stdin);

		//send responses to the client 
		buffer[strcspn(buffer, "\n")] = '\0'; //remove newline
		if (send(client_sock, buffer, strlen(buffer), 0) == -1) {
			perror("Send failure.\n");
			break;
		}

		//check if the server sent "bye"
		if (strcmp(buffer, "bye") == 0) {
			printf("Connection closed by server.\n");
			break;
		}
	}

	#ifdef _WIN32
	closesocket(client_sock);
	#else
	close(client_sock);
	#endif
}

int main() {
	#ifdef _WIN32
	if (init_windows_socker() != 0) {
		return 1;
	}
	#endif

	char ip_address[16];
	int port;
	get_server_details(ip_address, &port);

	int server_sock = initialize_server_socket(ip_address, port);
	if (server_sock == -1) { //in case of an error
		#ifdef _WIN32
		closesocket(server_sock);
		cleanup_windows_socket();
		#endif
		return 1;
	}

	//getting a client
	printf("Waiting for a clinet to connent...\n");
	int client_sock = accept_client_connection(server_sock);
	if (client_sock == -1) { //in case of failure
		#ifdef _WIN32
		closesocket(server_sock);
		cleanup_windows_socket();
		#endif
		return 1;
	}

	//assuming the client connected...
	printf("Client connected...\n");
	handle_client_communication(client_sock);

	//after the communication is completed
	#ifdef _WIN32
	closesocket(server_sock);
	cleanup_windows_socket();
	#else
	close(server_sock);
	#endif

	return 0;
}