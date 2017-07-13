#include "../include/chat.h"

const char *fileName = "./server1";
List *head = NULL;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int createServer(int count, struct queueId ids[]) {
	pthread_t threadConnect;
	int i;
	
	for (i = 0; i < count; i++) {
		ids[i].key = ftok(fileName, (char) i + 1);
		ids[i].msqId = msgget(ids[i].key, IPC_CREAT | 0666);
		if (ids[i].msqId < 0) {
			return -1;
		}
	}
	
	pthread_create(&threadConnect, NULL, listenConnection, &ids[0].msqId);
	return 0;
}

void *listenConnection(void *arguments) {
	int msqId = *((int*) arguments);
	struct msgServer msg;
	List *list = NULL, *delClient = NULL, *tempList = NULL;
	
	while(1) {
		msgrcv(msqId, &msg, sizeof(struct msgServer), HELLO_SERV, 0);
		pthread_mutex_lock(&mutex);
		printf("connect: %d\n", msg.connect);
		
		if (msg.connect == 0) {
			if (head != NULL) {
				delClient = searchElem(msg.typeClient, head);
				head = delElem(delClient, head);
			}
			sprintf(msg.hello, "GG BB\n");
			msg.type = HELLO_CLIENT + msg.typeClient;
			msgsnd(msqId, &msg, sizeof(struct msgServer), IPC_NOWAIT);
		} else {
			printf("New client with name %s is connected\n", msg.hello);
			if (head == NULL) {
				head = initList(msg.typeClient, msg.hello);
				list = head;
			} else {
				list = addElem(msg.typeClient, msg.hello, list);
			}
			snprintf(msg.hello, (size_t) 32, "Hello %s", list->name);
		}
		pthread_mutex_unlock(&mutex);
		tempList = head;
		
		while (tempList != NULL) {
			msg.type = HELLO_CLIENT + tempList->type;
			msgsnd(msqId, &msg, sizeof(struct msgServer), IPC_NOWAIT);
			tempList = tempList->next;
		}
		memset(msg.hello, 0, strlen(msg.hello));
	}
	pthread_exit(0);
}

void *listenConnectionClient(void *arguments) {
	int msqId = *((int*) arguments);
	struct msgServer msg;
	pid_t pid;
	List *list = NULL, *delClient = NULL;
	
	pid = getpid();
	while(1) {
		msgrcv(msqId, &msg, sizeof(struct msgServer), HELLO_CLIENT + pid, 0);
		pthread_mutex_lock(&mutex);
		if (msg.connect == 0) {
			if (head != NULL) {
				delClient = searchElem(msg.typeClient, head);
				printf("Clients with name %s is disconnected\n", delClient->name);
				head = delElem(delClient, head);
				
				if ((long) pid == msg.typeClient) {
					clearList(head);
					msg.type = CLOSE_CLIENT;
					pthread_mutex_unlock(&mutex);
					msgsnd(msqId, &msg, sizeof(struct msgServer), IPC_NOWAIT);
					break;
				}
			}
		} else {
			printf("New client with name %s is connected\n", msg.hello);
			if (head == NULL) {
				head = initList(msg.typeClient, msg.hello);
				list = head;
			} else {
				list = addElem(msg.typeClient, msg.hello, list);
			}
		}
		pthread_mutex_unlock(&mutex);
		memset(msg.hello, 0, strlen(msg.hello));
	}
	pthread_exit(EXIT_SUCCESS);
}

int connectToServer(int count, struct queueId ids[], char *nameClient, long type) {
	struct msgServer msg;
	pthread_t threadClient;
	int i;
	
	for (i = 0; i < count; i++) {
		ids[i].key = ftok(fileName, (char) i + 1);
		ids[i].msqId = msgget(ids[i].key, 0);
		if (ids[i].msqId < 0) {
			return -1;
		}
	}
	
	msg.type = HELLO_SERV;
	msg.typeClient = type;
	msg.connect = 1;
	sprintf(msg.hello, "%s", nameClient);
	msgsnd(ids[0].msqId, &msg, sizeof(struct msgServer), IPC_NOWAIT);
	pthread_create(&threadClient, NULL, listenConnectionClient, &ids[0].msqId);
	printf("%s\n", msg.hello);
	return 0;
}



void processClient(struct queueId queues[], long typeClient, char *nameClient) {
	/*List *listClients = NULL, *tempList = NULL;
	struct msgClient _msgClient;
	struct msgServer _msgServer;
	
	head = initList(typeClient, nameClient);
	
	while(1) {
		listClients = head;
		while(listClients != NULL) {
			printf("type = %ld\n", listClients->type);
			msgrcv(
				msqIds[1].msqId, 
				&msg, 
				sizeof(struct msgClient),
				listClients->type, 
				IPC_NOWAIT);
			if (errno != ENOMSG) {
				tempList = head;
				while(tempList != NULL) {
					msg.type = tempList->type;
					msgsnd(
						msqIds[2].msqId,
						&msg,
						sizeof(struct msgClient),
						0);
					tempList = tempList->next;
				}
			} else {
				errno = 0;
			}
			
			listClients = listClients->next;
		}
		
		pthread_mutex_unlock(&mutex);
		sleep(2);
		printf("wake up\n");
	}*/
}

int processServer(struct queueId msqIds[]) {
	List *listClients = NULL, *tempList = NULL;
	struct msgClient msg;
	struct msqid_ds qstatus;
	
	while(1) {
		pthread_mutex_lock(&mutex);
		listClients = head;
		
		while(listClients != NULL) {
			printf("type = %ld\n", listClients->type);
			msgrcv(
				msqIds[1].msqId, 
				&msg, 
				sizeof(struct msgClient),
				listClients->type, 
				IPC_NOWAIT);
			if (errno != ENOMSG) {
				tempList = head;
				while(tempList != NULL) {
					msg.type = tempList->type;
					msgsnd(
						msqIds[2].msqId,
						&msg,
						sizeof(struct msgClient),
						0);
					tempList = tempList->next;
				}
			} else {
				errno = 0;
			}
			listClients = listClients->next;
		}
		
		pthread_mutex_unlock(&mutex);
		sleep(1);
		printf("wake up\n");
	}
	return 0;
}

void disconnectClient(struct queueId queues[], char *nameClient, long type) {
	struct msgServer msg;
	
	msg.type = HELLO_SERV;
	msg.typeClient = type;
	msg.connect = 0;
	msgsnd(queues[0].msqId, &msg, sizeof(struct msgServer), IPC_NOWAIT);
	msgrcv(queues[0].msqId, &msg, sizeof(struct msgServer), CLOSE_CLIENT, 0);
	
	printf("BYE!\n");
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
