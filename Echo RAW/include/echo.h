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
#include <linux/if_packet.h>
#include <linux/if_ether.h>
#include <linux/if_arp.h>
#include <string.h>

#ifndef ECHO_H
#define ECHO_H
#define IP_ADDR "192.168.43.97"
#define PORT 8888
#define CLIENT_PORT 7777
#define DEFAULT_INTERFACE "wlp3s0"

extern int sizeBuf; 

int createServerUDP(struct sockaddr_in*);
int createSocketRaw(struct sockaddr_in*);
void addUdpHeader(unsigned char*);
int addIpHeader(int, unsigned char*);
int getNetworkInterface(int, struct ifreq**);
int clientRawEthernet();
int clientRaw(int);
int listenerUDP(int);
#endif
