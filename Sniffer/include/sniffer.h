#define _POSIX_C_SOURCE 1
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <pcap.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <net/ethernet.h>
#include <netinet/ip_icmp.h>
#include <netinet/udp.h>
#include <netinet/tcp.h>
#include <netinet/ip.h>
 
#ifndef SNIFFER_H
#define SNIFFER_H

void processPacket(u_char *, const struct pcap_pkthdr *, const u_char *);
void printEthernetHeader(const u_char *, int);
void printIpHeader(const u_char *, int);
int checkSum(unsigned short *, unsigned int);
void computeIpChecksum(struct iphdr *);
void PrintData(const u_char *, int);
#endif
