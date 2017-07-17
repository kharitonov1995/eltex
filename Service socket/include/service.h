#define _POSIX_C_SOURCE 1
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <pthread.h>
#include <time.h>
#include <string.h>
#include <fcntl.h>

#ifndef SERVICE_H
#define SERVICE_H
#define IP_ADDR "127.0.0.1"
#define PORT 8888
#define PUT_CLIENT 1L
#define GET_CLIENT 2L
#define ARRAY_SIZE(array) sizeof(array)/sizeof(array[0]);

struct queueId {
	key_t key;
	int msqId;
};

struct msgServer {
	long type;
	char message[32];
	struct sockaddr_in addrClient;
};

int createServerTCP(struct sockaddr_in*);
int createServerUDP(struct sockaddr_in*, struct queueId[]);
int connectToServerTCP(struct sockaddr_in*);
int connectToServerUDP(struct sockaddr_in*, char*);
int listenerTCP(int, socklen_t);
int listenerUDP(struct queueId[], int);
void *listenerQueue(void*);
void *threadClient(void*);
void getCurrentTime(char*, const int);
#endif
