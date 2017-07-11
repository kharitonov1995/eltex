#include "../include/chat.h"

struct args {
	List **list;
	int msq;	
};

int createServer(int *serverMsq, int *clientsMsq, List **head) {
	const char *fileName = "./server";
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
	
	msg.type = 1L;
	while(1) {
		msgrcv(arg->msq, &msg, sizeof(struct msgServer), msg.type, 0);
		printf("New client with name %s is connected\n", msg.hello);
		if (*(arg->list) == NULL) {
			*(arg->list) = initList(msg.pid, msg.hello);
		} else {
			*(arg->list) = addElem(msg.pid, msg.hello, *(arg->list));
		}
	}
	pthread_exit(0);
}

pid_t connectToServer(int *fifoServer) {
	pid_t pidServer = 0;
	
	return pidServer;
}

void processClient(int pidServer, int fifoServer) {
	
}

int processServer(int fifoServer) {
	
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
