#include "../include/echo.h"

int main() {
	int sock = 0;
	struct sockaddr_in addrServer;
		
	sock = createServerUDP(&addrServer);
	printf("Created UDP server\n");
	listenerUDP(sock);
	
	exit(EXIT_SUCCESS);
}
