#include "../include/chat.h"

int main() {
	List *list = NULL, *temp = NULL;
	int serverMsq = 0, clientsMsq = 0;
	
	if (createServer(&serverMsq, &clientsMsq, &list) < 0) {
		printf("Can't create server\n");
		msgctl(serverMsq, IPC_RMID, NULL);
		msgctl(clientsMsq, IPC_RMID, NULL);
		exit(EXIT_FAILURE);
	}
	while(1) {
		sleep(2);
		if (head != NULL) break;
	}
	processServer(clientsMsq);
	msgctl(serverMsq, IPC_RMID, NULL);
	msgctl(clientsMsq, IPC_RMID, NULL);
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
