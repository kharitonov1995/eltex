#include "../include/chat.h"

int main() {
	List *list = NULL;
	int serverMsq = 0, clientsMsq = 0;
	
	if (createServer(&serverMsq, &clientsMsq, &list) < 0) {
		printf("Can't create server\n");
		msgctl(serverMsq, IPC_RMID, NULL);
		msgctl(clientsMsq, IPC_RMID, NULL);
		exit(EXIT_FAILURE);
	}
	while(1) {
		getchar();
		if (list != NULL) {
			printf("%s\n%ld", list->name, list->type); 
			list = list->next;
		}
	}
	msgctl(serverMsq, IPC_RMID, NULL);
	msgctl(clientsMsq, IPC_RMID, NULL);
	exit(EXIT_SUCCESS);
}
