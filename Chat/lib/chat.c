#include "../include/chat.h"

const char *fileName = "./server";
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
		
		if (msg.connect == 0) {
			printf("Client with name %s is disconnected\n", msg.hello);
			if (head != NULL) {
				delClient = searchElem(msg.typeClient, head);
				delElem(delClient, &head);
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
			
			msg.type = GET_CLIENTS;
			msg.countClients = sizeList(head);
			msgsnd(msqId, &msg, sizeof(struct msgServer), IPC_NOWAIT);
			
			tempList = head;
			while (tempList != NULL) {
				bzero(msg.hello, strlen(msg.hello));
				msg.typeClient = tempList->type;
				memcpy(msg.hello, tempList->name, strlen(tempList->name));
				msgsnd(msqId, &msg, sizeof(struct msgServer), IPC_NOWAIT);
				tempList = tempList->next;
			}
			
			snprintf(msg.hello, (size_t) 32, "%s", list->name);
		}
		pthread_mutex_unlock(&mutex);
		
		tempList = head;
		while (tempList != NULL) {
			msg.type = HELLO_CLIENT + tempList->type;
			msgsnd(msqId, &msg, sizeof(struct msgServer), IPC_NOWAIT);
			tempList = tempList->next;
		}
		
		bzero(&msg, sizeof(struct msgServer));
	}
	pthread_exit(0);
}

void *listenConnectionClient(void *arguments) {
	int msqId = *((int*) arguments);
	int count = 0;
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
				delElem(delClient, &head);
				
				if ((long) pid == msg.typeClient) {
					if (head != NULL)
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
				msgrcv(msqId, &msg, sizeof(struct msgServer), GET_CLIENTS, 0);
				
				count = msg.countClients - 1;
				do {
					msgrcv(msqId, &msg, sizeof(struct msgServer), GET_CLIENTS, 0);
					if (msg.typeClient != pid) {
						list = addElem(msg.typeClient, msg.hello, list);
						count--;
					}
				} while (count > 0);
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
	struct msgClient msg;
	pid_t pid = 0;
	
	pid = fork();
	msg.type = typeClient;
	if (pid < 0) {
		perror("fork()");
		exit(EXIT_FAILURE);
	}
	
	if (pid == 0) {
		while (1) {
			msgrcv(
				queues[2].msqId, 
				&msg, 
				sizeof(struct msgClient),
				typeClient, 
				0);
			printf("message: %s\n", msg.message);
		}
	} else if (pid > 0) {
		while(1) {
			printf("Input:\n");
			bzero(msg.message, strlen(msg.message));
			if (fgets(msg.message, 255, stdin) == NULL) {
				perror("fgets()");
				exit(EXIT_FAILURE);
			}
			if (msg.message[0] == 27) {
				kill(pid, SIGTERM);
				break;
			}
			msgsnd(
				queues[1].msqId,
				&msg,
				sizeof(struct msgClient),
				0);
			printf("sending: %s\n", msg.message);
		}
	} 
	
}

int processServer(struct queueId msqIds[]) {
	List *listClients = NULL, *tempList = NULL;
	struct msgClient msg;
	
	while(1) {
		pthread_mutex_lock(&mutex);
		listClients = head;
		
		while(listClients != NULL) {
			msgrcv(
				msqIds[1].msqId, 
				&msg, 
				sizeof(struct msgClient),
				listClients->type, 
				IPC_NOWAIT);
			if (errno != ENOMSG) {
				printf("message from: %ld\n", listClients->type);
				tempList = head;
				while(tempList != NULL) {
					if (listClients->type != tempList->type) {
						msg.type = tempList->type;
						msgsnd(
							msqIds[2].msqId,
							&msg,
							sizeof(struct msgClient),
							0);
					}
					tempList = tempList->next;
				}
			} else {
				errno = 0;
			}
			listClients = listClients->next;
		}
		
		pthread_mutex_unlock(&mutex);
		sleep(1);
	}
	return 0;
}

void disconnectClient(struct queueId queues[], char *nameClient, long type) {
	struct msgServer msg;
	
	msg.type = HELLO_SERV;
	memcpy(msg.hello, nameClient, (size_t) 32);
	msg.typeClient = type;
	msg.connect = 0;
	msgsnd(queues[0].msqId, &msg, sizeof(struct msgServer), IPC_NOWAIT);
	msgrcv(queues[0].msqId, &msg, sizeof(struct msgServer), CLOSE_CLIENT, 0);
	
	printf("%s\n", msg.hello);
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
