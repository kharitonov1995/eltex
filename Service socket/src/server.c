#include "../include/service.h"
const char *fileName =  "/home/artem/Документы/eltex/Service socket/server";

int main(int argc, char *argv[]) {
	struct sockaddr_in addrServer;
	struct queueId *queues = NULL;
	int typeServer = 0, sock = 0, i, status = 0, sockets[2];
	
	if (argc < 2) {
		printf("Please select type server:\n1 - UDP\n2 - TCP\n3 - TCP/UDP");
		exit(EXIT_FAILURE);
	}
	
	typeServer = atoi(argv[1]);
	
	if (typeServer == 1) {
		/*printf("Select type UDP server:\n1 - Queue\n2 - Threads\n");
		if (scanf("%d", &typeServer) == EOF) {
			perror("scanf()");
			exit(EXIT_FAILURE);
		}*/
		/*if (typeServer == 1) {*/
			queues = malloc(sizeof(struct queueId) * 2);
			for (i = 0; i < 2; i++) {
				queues[i].key = ftok(fileName, (char) i + 1);
				queues[i].msqId = msgget(queues[i].key, IPC_CREAT | 0666);
				if (queues[i].msqId < 0) {
					perror("msgget()");
					exit(EXIT_FAILURE);
				}
			/*}*/
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
		status = listenerTCP(sock);
		if (status < 0) {
			exit(EXIT_FAILURE);
		}
	} else if (typeServer == 3) {
		sockets[0] = createServerTCP(&addrServer);
		if (sockets[0] < 0) {
			exit(EXIT_FAILURE);
		}
		sockets[1] = createServerUDP(&addrServer, queues);
		if (sockets[1] < 0) {
			exit(EXIT_FAILURE);
		}
		/*type: 1 - select; 2 - poll; 3 - epoll*/
		status = listenerTCP_UDP(sockets, 3);
		if (status < 0) {
			exit(EXIT_FAILURE);
		}
	}
	
	if (queues != NULL) 
		free(queues);
	close(sock);
	exit(EXIT_SUCCESS);
}
