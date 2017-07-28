#define _POSIX_C_SOURCE 1
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>
#include <netinet/udp.h>
#include <netinet/ip.h>
#include <net/if.h>
#include <linux/if_ether.h>
#include <linux/if_arp.h>
#include <string.h>

#ifndef ECHO_H
#define ECHO_H
#define IP_ADDRD "192.168.2.1"
#define IP_ADDR_SERVER "192.168.2.59"
#define IP_ADDRS "192.168.2.59"
#define PORT_SERVER 7373
#define PORT 7777
#define CLIENT_PORT 8888
#define WIFI_INTERFACE "wlp3s0"
#define ETH_INTERFACE "enp4s0"
#define LO_INTERFACE "lo"

extern int sizeBuf; 

int createServerUDP(struct sockaddr_in*);
int createSocketRaw(struct sockaddr_in*);
void addUdpHeader(unsigned char*);
int addIpHeader(int, unsigned char*);
int clientRawEthernet();
int clientRaw(int);
unsigned short checkSum(unsigned short*, unsigned int);
int listenerUDP(int);
#endif
