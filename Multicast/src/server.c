#include "../include/multicast.h"

int main() {
	struct sockaddr_in addrClient;
	int sock = 0, sizeMessage = 32, status = 0;
	char *message;
	socklen_t sizeAddr;
	
	sizeAddr = sizeof(addrClient);
	message = malloc(sizeof(char) * sizeMessage);
	sock = createServerUDP(&addrClient);
	if (sock < 0) {	
		exit(EXIT_FAILURE);
	}	
	
	while (1) {
		getCurrentTime(message, sizeMessage);
		status = sendto(sock, message, sizeMessage, 0, &addrClient, sizeAddr);
		if (status < 0) {
			perror("sendto()");
			exit(EXIT_FAILURE);
		}
		bzero(message, sizeMessage);
		sleep(3);
	}
	
	close(sock);
	free(message);
	exit(EXIT_SUCCESS);
}
