#include "../include/chat.h"

int main(int argc, char *argv[]) {
	char *nameClient = NULL;
	int count = 3;
	struct queueId *queues;
	long type = 0L;
	
	queues = malloc(sizeof(struct queueId) * count);
	
	if (argc < 2) {
		printf("Please input name\n");
		exit(EXIT_FAILURE);
	}
	
	nameClient = argv[1];
	type = (long) getpid();
	printf("type = %ld\n", type);
	
	connectToServer(count, queues, nameClient, type);
	processClient(queues, type, nameClient);
	disconnectClient(queues, nameClient, type);
	
	exit(EXIT_SUCCESS);
}
