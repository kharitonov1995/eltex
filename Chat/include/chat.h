#define _POSIX_C_SOURCE 1
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <pthread.h>

#include "list.h"

#ifndef CHAT_H
#define CHAT_H
#define HELLO_SERV 1L
#define HELLO_CLIENT 2L
#define CLOSE_SERV 3L
#define CLOSE_CLIENT 4L
#define GET_CLIENTS 5L

extern List *head;
extern pthread_mutex_t mutex;

struct queueId {
	key_t key;
	int msqId;
};

struct msgServer {
	long type;
	int typeClient;
	int connect;
	int countClients;
	char hello[32];
};

struct msgClient {
	long type;
	char message[255];
};

int createServer(int, struct queueId[]);
void *listenConnection(void*);
void *listenConnectionClient(void*);
int connectToServer(int, struct queueId[], char*, long);
void disconnectClient(struct queueId[], char*, long);
void processClient(struct queueId[], long, char*);
int processServer(struct queueId[]);
char *getCurrentTime();
#endif
