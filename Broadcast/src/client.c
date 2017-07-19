#include "../include/broadcast.h"

int main(int argc, char *argv[]) {
	struct sockaddr_in addrClient;
	int sock, sizeMessage = 32;
	socklen_t sizeAddr;
	char *buffer = NULL;
	
	sizeAddr = sizeof(addrClient);
	buffer = calloc(sizeMessage, sizeof(char));
	
	sock = connectToServerUDP(&addrClient);
	recvfrom(sock, buffer, sizeMessage, 0, &addrClient, &sizeAddr);
	printf("%s\n", buffer);

	exit(EXIT_SUCCESS);
}
