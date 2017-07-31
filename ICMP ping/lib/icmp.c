#include "../include/icmp.h"

int sizeBuf = 64;

int createSock() {
	int sock = 0;

	sock = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
	if(sock < 0) {
		perror("socket()");
		return -1;
	}
	
	return sock;
}

int addIpHeader(int sock, uint32_t ipAddrD, unsigned char *buffer) {
	int status = 0, flag = 1;
	struct iphdr *iph = NULL;

	if (sock > 0) {
		status = setsockopt(sock, IPPROTO_IP, IP_HDRINCL, &flag, sizeof(flag));
		if(status < 0) {
			perror("setsockopt(IP_HDRINCL)");
			return -1;
		}
	}

	iph = malloc(sizeof(struct iphdr));
	bzero(iph, sizeof(struct iphdr));
	
	iph->ihl = 5;
	iph->version = 4;
	iph->tos = 0;
	iph->tot_len = htons(sizeof(struct iphdr) + sizeof(struct icmphdr));
	iph->id = htons(12345);
	iph->frag_off = 0;
	iph->ttl = 65;
	iph->protocol = IPPROTO_ICMP;
	iph->check = 0;
	iph->saddr = inet_addr(IP_ADDR);
	iph->daddr = ipAddrD;
		
	iph->check = checkSum((unsigned short*) iph, sizeof(struct iphdr));
	memcpy(buffer, iph, sizeof(struct iphdr));
	free(iph);
	return 0;
}

int addIcmpHeader(unsigned char *buffer) {
	struct icmphdr *icmp = NULL;
	
	icmp = malloc(sizeof(struct icmphdr));
	
	icmp->type = ICMP_ECHO;
    icmp->code = 0;
    icmp->un.echo.sequence = 0;
    icmp->un.echo.id = getpid();
    icmp->checksum = checkSum((unsigned short*) icmp, sizeof(struct icmphdr));
	
	memcpy(buffer, icmp, sizeof(struct icmphdr));
	free(icmp);
	return 0;
}

unsigned short checkSum(unsigned short *addr, unsigned int count) {
	unsigned long sum = 0;

	while (count > 1) {
		sum += *addr++;
		count -= 2;
	}

	if(count > 0) {
		sum += ((*addr) & htons(0xFF00));
	}

	while (sum >> 16) {
		sum = (sum & 0xffff) + (sum >> 16);
	}

	sum = ~sum;
	return ((unsigned short)sum);
}
