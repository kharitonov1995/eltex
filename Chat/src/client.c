#include "../include/chat.h"

int main(int argc, char *argv[]) {
	char *nameClient = NULL;
	List *msg;
	if (argc < 2) {
		printf("Please input name\n");
		exit(EXIT_FAILURE);
	}
	
	nameClient = argv[1];
	connectToServer(nameClient);
	key_t key;
	int msqId;
	const char *fileName = "./server"; 
	key = ftok(fileName, 'C');
	msqId = msgget(key, 0666);
	msg = initList((long) getpid(), nameClient);
	sprintf(msg->message, "12312412412412");
	msgsnd(msqId, &msg, sizeof(List), 0);
	printf("type = %ld\n", msg->type);
	getchar();
	/*printf("клиент: %s\n", msg->message);*/
	/*msgrcv(msqId, &msg, sizeof(struct msgServer), book.type, 0); 
	printf("message = %s\n", book.str);*/
	exit(EXIT_SUCCESS);
}
