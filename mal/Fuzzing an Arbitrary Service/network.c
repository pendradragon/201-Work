#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <openssl/evp.h>

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "libcrypto.lib")

#define BUFFER_SIZE 256
#define PORT 12345

// Function to compute the hash
void calculate_hash(const char* algorithm, const char* input, char* output) {
    EVP_MD_CTX* mdctx = EVP_MD_CTX_new();
    const EVP_MD* md = EVP_get_digestbyname(algorithm);

    if (!md) {
        sprintf(output, "Error: Unknown algorithm '%s'.\n", algorithm);
        EVP_MD_CTX_free(mdctx);
        return;
    }

    unsigned char hash[EVP_MAX_MD_SIZE];
    unsigned int hash_len;

    EVP_DigestInit_ex(mdctx, md, NULL);
    EVP_DigestUpdate(mdctx, input, strlen(input));
    EVP_DigestFinal_ex(mdctx, hash, &hash_len);

    // Convert hash bytes to a hexadecimal string
    for (unsigned int i = 0; i < hash_len; i++) {
        sprintf(output + (i * 2), "%02x", hash[i]);
    }
    output[hash_len * 2] = '\0'; // Null-terminate the string

    EVP_MD_CTX_free(mdctx);
}

// Function to process client requests
void process_request(char* request, char* response) {
    char command[BUFFER_SIZE];
    char argument[BUFFER_SIZE];

    // Intentionally insecure parsing (buffer overflow potential)
    sscanf(request, "%s %s", command, argument);

    if (strcmp(command, "sha512") == 0) {
        calculate_hash("sha512", argument, response);
    }
    else if (strcmp(command, "sha384") == 0) {
        calculate_hash("sha384", argument, response);
    }
    else if (strcmp(command, "sha256") == 0) {
        calculate_hash("sha256", argument, response);
    }
    else {
        sprintf(response, "Error: Unknown command '%s'.\n", command);
    }
}

int main() {
    WSADATA wsa;
    SOCKET server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    int client_addr_len = sizeof(client_addr);
    char buffer[BUFFER_SIZE];
    char response[BUFFER_SIZE];

    printf("Initializing Winsock...\n");
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        printf("Failed to initialize Winsock. Error Code: %d\n", WSAGetLastError());
        return 1;
    }

    printf("Creating socket...\n");
    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        printf("Socket creation failed. Error Code: %d\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    printf("Binding socket...\n");
    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        printf("Bind failed. Error Code: %d\n", WSAGetLastError());
        closesocket(server_socket);
        WSACleanup();
        return 1;
    }

    printf("Listening for connections...\n");
    if (listen(server_socket, 3) == SOCKET_ERROR) {
        printf("Listen failed. Error Code: %d\n", WSAGetLastError());
        closesocket(server_socket);
        WSACleanup();
        return 1;
    }

    printf("Waiting for incoming connections...\n");
    client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_addr_len);
    if (client_socket == INVALID_SOCKET) {
        printf("Accept failed. Error Code: %d\n", WSAGetLastError());
        closesocket(server_socket);
        WSACleanup();
        return 1;
    }

    printf("Client connected.\n");

    while (1) {
        memset(buffer, 0, BUFFER_SIZE);
        memset(response, 0, BUFFER_SIZE);

        // Receive client request
        int recv_size = recv(client_socket, buffer, BUFFER_SIZE, 0);
        if (recv_size == SOCKET_ERROR) {
            printf("Recv failed. Error Code: %d\n", WSAGetLastError());
            break;
        }
        else if (recv_size == 0) {
            printf("Client disconnected.\n");
            break;
        }

        buffer[recv_size] = '\0'; // Null-terminate the received string
        printf("Received: %s\n", buffer);

        // Process the request and compute the response
        process_request(buffer, response);
        printf("Responding with: %s\n", response);

        // Send the response to the client
        send(client_socket, response, strlen(response), 0);
    }

    closesocket(client_socket);
    closesocket(server_socket);
    WSACleanup();

    return 0;
}