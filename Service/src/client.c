#include "../include/service.h"

int main() {
	pid_t pidServer;
	int fifoServer;
	
	pidServer = connectToServer(&fifoServer);
	if (pidServer < 0) {
		printf("Can't connect to server\n");
		exit(EXIT_FAILURE);
	}
	processClient(pidServer, fifoServer);
	close(fifoServer);
	exit(EXIT_SUCCESS);
}
