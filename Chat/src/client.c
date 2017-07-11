#include "../include/chat.h"

int main() {
	const char *fileName = "./server";	
	struct msgServer msg;
	int msqId = 0;
	key_t key = 0;
	
	key = ftok(fileName, 'S');
	msqId = msgget(key, 0666);
	msg.type = 1L;
	msg.pid = getpid();
	sprintf(msg.hello, "HELLO");
	msgsnd(msqId, &msg, sizeof(struct msgServer), IPC_NOWAIT);
	/*msgrcv(msqId, &msg, sizeof(struct msgServer), book.type, 0); 
	printf("message = %s\n", book.str);*/
	exit(EXIT_SUCCESS);
}
