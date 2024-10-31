// for setting up the socket the server will use

#ifndef WINDOWS_SOCKET_H
#define WINDOWS_SOCKET_H

#ifdef _WIN32
#include <winsock2.h>
#include <stdio.h>

#pragma comment(lib, "ws2_32.lib")

//initalize the Windows Socket
int init_windows_socet() {
	WSADATA wsaData; 
	int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (result != 0) {
		printf("WSAStartup failed: %d\n.", result);
		return -1;
	}
	return 0;
}

//clean up the socket after everything is done
void cleanup_windows_socket() {
	WSACleanup();
}

#endif //_WIN32
#endif //WINDOWS_SOCKET_H