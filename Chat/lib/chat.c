#include "../include/chat.h"

const char *fileName = "./server1";
List *head = NULL;

struct args {
	List **list;
	int msq;	
};

int createServer(int *serverMsq, int *clientsMsq, List **head) {
	key_t key = 0;
	pthread_t threadConnect;
	struct args *arg;
	
	key = ftok(fileName, 'S');
	printf("%x\n", key);
	*serverMsq = msgget(key, IPC_CREAT | 0666);
	if (*serverMsq == 0)
		return -1;
		
	key = ftok(fileName, 'C');
	*clientsMsq = msgget(key, IPC_CREAT | 0666);
	if (*clientsMsq == 0)
		return -1;
	
	arg = malloc(sizeof(struct args));
	arg->list = head;
	arg->msq = *serverMsq;
	pthread_create(&threadConnect, NULL, listenConnection, arg);
	return 0;
}

void *listenConnection(void *arguments) {
	struct args *arg = (struct args*) arguments;
	struct msgServer msg;
	
	while(1) {
		msg.type = HELLO_SERV;
		msgrcv(arg->msq, &msg, sizeof(struct msgServer), msg.type, 0);
		printf("New client with name %s is connected\n", msg.hello);
		if (head == NULL) {
			head = initList(msg.pid, msg.hello);
			*(arg->list) = head;
		} else {
			*(arg->list) = addElem(msg.pid, msg.hello, *(arg->list));
		}
		memset(msg.hello, 0, strlen(msg.hello));
		
		msg.type = HELLO_CLIENT;
		sprintf(msg.hello, "Hello from server\n");
		msgsnd(arg->msq, &msg, sizeof(struct msgServer), 0);
		memset(msg.hello, 0, strlen(msg.hello));
	}
	pthread_exit(0);
}

int connectToServer(char *nameClient) {
	struct msgServer msg;
	int msqId = 0;
	key_t key;
	
	key = ftok(fileName, 'S');
	msqId = msgget(key, 0666);
	msg.type = 1L;
	msg.pid = (long) getpid();
	sprintf(msg.hello, "%s", nameClient);
	msgsnd(msqId, &msg, sizeof(struct msgServer), 0);
	msgrcv(msqId, &msg, sizeof(struct msgServer), HELLO_CLIENT, 0);
	
	printf("%s\n", msg.hello);
	return 0;
}

void processClient(int pidServer, int fifoServer) {
	
}

int processServer(int clientsMsq) {
	List *listClients = NULL, *tempList = NULL;
	while(1) {
		listClients = head;
		while(listClients != NULL) {
			/*getchar();*/
			printf("type = %ld\n", listClients->type);
			msgrcv(
				clientsMsq, 
				&listClients, 
				sizeof(List),
				listClients->type, 
				IPC_NOWAIT);
			if (errno == ENOMSG) {	
				listClients = listClients->next;
			} else {
				printf("принято: %s\n", listClients->message);
				tempList = head;
				while(tempList != NULL) {
					/*memcpy(
						tempList->message,
						listClients->message,
						strlen(listClients->message));*/
					sprintf(tempList->message, "qweqe");
					msgsnd(clientsMsq, &tempList, sizeof(List), IPC_NOWAIT);
				}
			}
		}
		sleep(2);
		printf("wake up\n");
	}
	return 0;
}

char *getCurrentTime() {
	const int sizeBuf = 16;
	time_t rawtime;
	struct tm *timeinfo;
	char *buf = calloc(sizeBuf, sizeof(char));

	time(&rawtime);
	timeinfo = localtime(&rawtime);
	strftime(buf, sizeBuf, "%d.%m.%y %H:%M", timeinfo);
	
	return buf;
}
