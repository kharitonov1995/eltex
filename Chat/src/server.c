#include "../include/chat.h"

int main() {
	List *list = NULL, *temp = NULL;
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
	/*while(1) {
		sleep(2);
		pthread_mutex_lock(&mutex);
		list = head;
		while(list != NULL) {
			printf("type = %ld\n", list->type);
			list = list->next;
		}
		pthread_mutex_unlock(&mutex);
	}*/
	for (i = 0; i < countQueue; i++) {
		msgctl(queues[i].msqId, IPC_RMID, NULL);
	}
	free(queues);
	exit(EXIT_SUCCESS);
}
/*while(1) {
		getchar();
		temp = head;
		while(list != NULL) {
			printf("%s\n%ld\n", temp->name, temp->type); 
			temp = temp->next;
		}
	}*/
