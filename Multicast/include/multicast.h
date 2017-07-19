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
#include <time.h>
#include <string.h>

#ifndef MULTICAST_H
#define MULTICAST_H
#define IP_ADDR "192.168.43.97"
#define MULTICAST_IP_ADDR "225.10.10.1"
#define PORT 8888
#define ARRAY_SIZE(array) sizeof(array)/sizeof(array[0]);

int createServerUDP(struct sockaddr_in*);
int connectToServerUDP(struct sockaddr_in*);
int listenerUDP(int);
void getCurrentTime(char*, const int);
#endif
