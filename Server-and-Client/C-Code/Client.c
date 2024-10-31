#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#include "window_socket.h"
#pragma comment(lib, "ws2_32.lib")
#else
#include <unistd.h>
#include <arpa/inet.h>
#endif

#define BUFFER_SIZE 1024

// Function declarations
void get_server_details(char* ip_address, int* port);
int connect_to_server(const char* ip_address, int port);
void chat_with_server(int client_sock);

int main() {
#ifdef _WIN32
    if (init_windows_socket() != 0) {
        return 1;
    }
#endif

    char ip_address[16];
    int port;
    get_server_details(ip_address, &port);

    int client_sock = connect_to_server(ip_address, port);
    if (client_sock == -1) {
#ifdef _WIN32
        cleanup_windows_socket();
#endif
        return 1;
    }

    printf("Connected to server successfully.\n");
    chat_with_server(client_sock);

    // Cleanup
#ifdef _WIN32
    closesocket(client_sock);
    cleanup_windows_socket();
#else
    close(client_sock);
#endif

    return 0;
}

// Prompt user for server IP and port
void get_server_details(char* ip_address, int* port) {
    printf("Enter server IP address: ");
    scanf("%15s", ip_address);
    printf("Enter server port number: ");
    scanf("%d", port);
}

// Connect to the server
int connect_to_server(const char* ip_address, int port) {
    int client_sock;
    struct sockaddr_in server_addr;

    // Create socket
    if ((client_sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Socket creation failed");
        return -1;
    }

    // Configure server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    if (inet_pton(AF_INET, ip_address, &server_addr.sin_addr) <= 0) {
        perror("Invalid address/Address not supported");
#ifdef _WIN32
        closesocket(client_sock);
#else
        close(client_sock);
#endif
        return -1;
    }

    // Connect to server
    if (connect(client_sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("Connection failed");
#ifdef _WIN32
        closesocket(client_sock);
#else
        close(client_sock);
#endif
        return -1;
    }

    return client_sock;
}

// Chat with the server until "bye" is sent
void chat_with_server(int client_sock) {
    char buffer[BUFFER_SIZE];

    while (1) {
        // Prompt user for message
        printf("You:> ");
        fgets(buffer, BUFFER_SIZE, stdin);
        buffer[strcspn(buffer, "\n")] = '\0'; // Remove newline

        // Send message to server
        if (send(client_sock, buffer, strlen(buffer), 0) == -1) {
            perror("Send failed");
            break;
        }

        // Check if client wants to end chat
        if (strcmp(buffer, "bye") == 0) {
            printf("Connection closed by client.\n");
            break;
        }

        // Receive response from server
        memset(buffer, 0, BUFFER_SIZE);
        int recv_len = recv(client_sock, buffer, BUFFER_SIZE - 1, 0);
        if (recv_len == -1) {
            perror("Receive failed");
            break;
        }

        buffer[recv_len] = '\0';
        printf("Server:> %s\n", buffer);

        // Check if server closed the connection
        if (strcmp(buffer, "bye") == 0) {
            printf("Connection closed by server.\n");
            break;
        }
    }
}
