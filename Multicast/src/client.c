#include "../include/multicast.h"

int main(int argc, char *argv[]) {
	struct sockaddr_in addrClient;
	int sock, sizeMessage = 32;
	socklen_t sizeAddr;
	char *buffer = NULL;
	
	sizeAddr = sizeof(addrClient);
	buffer = calloc(sizeMessage, sizeof(char));
	
	sock = connectToServerUDP(&addrClient);
	recvfrom(sock, buffer, sizeMessage, 0, &addrClient, &sizeAddr);
	printf("Recv %s from %s:%d \n", 
			buffer, 
			inet_ntoa(addrClient.sin_addr), 
			ntohs(addrClient.sin_port));
	
	free(buffer);
	close(sock);
	exit(EXIT_SUCCESS);
}
