#include "../include/service.h"

int main(int argc, char *argv[]) {
	struct sockaddr_in addrServer;
	struct msgServer msg;
	int sock, sizeMessage, i, count = 10000, typeServer = 0;
	char *nameClient = NULL;
	socklen_t sizeAddr;
	char *buffer = NULL;
	pthread_t *threads = NULL;
	
	if (argc < 3) {
		printf("Please input name and type server:\n1 - UDP\n2 - TCP");
		exit(EXIT_FAILURE);
	}
	
	nameClient = argv[1];
	typeServer = atoi(argv[2]);
	sizeAddr = sizeof(addrServer);
	sizeMessage = ARRAY_SIZE(msg.message);
	buffer = calloc(sizeMessage, sizeof(char));
	
	if (typeServer == 1) {
		sock = connectToServerUDP(&addrServer, nameClient);
		recvfrom(sock, buffer, sizeMessage, 0, &addrServer, &sizeAddr);
		printf("%s\n", buffer);
	} else if (typeServer == 2) {
		threads = malloc(sizeof(pthread_t) * count);
		snprintf(msg.message, sizeMessage, "%s", nameClient);
		for (i = 0; i < count; i++) {
			msg.type = i;
			pthread_create(&threads[i], NULL, threadClient, &msg);
		}
	}
		
	exit(EXIT_SUCCESS);
}
