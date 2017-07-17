#include "../include/service.h"
const char *fileName =  "/home/artem/Документы/eltex/Service socket/server";

int main(int argc, char *argv[]) {
	struct sockaddr_in addrServer;
	struct queueId *queues;
	int typeServer = 0, sock = 0, i, status = 0;
	
	if (argc < 2) {
		printf("Please select type server:\n1 - Queues\n2 - Threads\n");
		exit(EXIT_FAILURE);
	}
	
	typeServer = atoi(argv[1]);
	
	if (typeServer == 1) {
		queues = malloc(sizeof(struct queueId) * 2);
		for (i = 0; i < 2; i++) {
			queues[i].key = ftok(fileName, (char) i + 1);
			queues[i].msqId = msgget(queues[i].key, IPC_CREAT | 0666);
			if (queues[i].msqId < 0) {
				perror("msgget()");
				exit(EXIT_FAILURE);
			}
		}
		sock = createServerUDP(&addrServer, queues);
		if (sock < 0) {	
			exit(EXIT_FAILURE);
		}	
		if (listenerUDP(queues, sock) < 0) {
			exit(EXIT_FAILURE);
		}
	} else if (typeServer == 2) {
		sock = createServerTCP(&addrServer);
		if (sock < 0) {
			exit(EXIT_FAILURE);
		}
		status = listenerTCP(sock, (socklen_t) sizeof(addrServer));
		if (status < 0) {
			exit(EXIT_FAILURE);
		}
	}
	exit(EXIT_SUCCESS);
}
