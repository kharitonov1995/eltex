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

struct msgServer {
	long type;
	int pid;
	char hello[32];
};

int createServer(int*, int*, List**);
void *listenConnection(void*);
pid_t connectToServer(int*);
void processClient(int, int);
int processServer(int);
char *getCurrentTime();
#endif
