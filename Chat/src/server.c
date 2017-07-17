#include "../include/chat.h"

int main() {
	int countQueue = 3, i;
	struct queueId *queues = NULL;
	
	queues = malloc(sizeof(struct queueId) * countQueue);
	
	if (createServer(countQueue, queues) < 0) {
		printf("Can't create server\n");
		for (i = 0; i < countQueue; i++) {
			msgctl(queues[i].msqId, IPC_RMID, NULL);
		}
		exit(EXIT_FAILURE);
	}
	processServer(queues);
	for (i = 0; i < countQueue; i++) {
		msgctl(queues[i].msqId, IPC_RMID, NULL);
	}
	free(queues);
	exit(EXIT_SUCCESS);
}
