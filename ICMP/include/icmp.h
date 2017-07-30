#define _POSIX_C_SOURCE 1
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <net/ethernet.h>
#include <netinet/ip_icmp.h>
#include <netinet/udp.h>
#include <netinet/tcp.h>
#include <netinet/ip.h>
 
#ifndef ICMP_H
#define ICMP_H
#define IP_ADDR "192.168.0.21"
#define PORT 7373
#define IP_ADDR_LEN 4

extern int sizeBuf;

int createSock();
int addIpHeader(int, uint32_t, unsigned char *);
int addIcmpHeader(unsigned char *);
unsigned short checkSum(unsigned short *, unsigned int);
#endif
