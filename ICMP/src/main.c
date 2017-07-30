#include "../include/icmp.h"

int main(int argc, char *argv[]) {
	int sock = 0, status = 0;
	struct sockaddr_in addr;
	unsigned char *buffer = NULL;
	uint32_t ipAddrD = 0;
	socklen_t sizeAddr = 0;
	
	if (argc < 2) {
		printf("Please input distination ip addres...\n");
		exit(EXIT_FAILURE);
	}
	
	sock = createSock();
	if (sock < 0) {
		exit(EXIT_FAILURE);
	}
	
	buffer = malloc(sizeof(char) * sizeBuf);
	
	ipAddrD =  inet_addr(argv[1]);
	
	status = addIpHeader(sock, ipAddrD, buffer);
	if (status < 0) {
		free(buffer);
		close(sock);
		exit(EXIT_FAILURE);
	}
	
	status = addIcmpHeader(buffer + sizeof(struct iphdr));
	if (status < 0) {
		free(buffer);
		close(sock);
		exit(EXIT_FAILURE);
	}
	
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = ipAddrD;
	
	sizeAddr = sizeof(addr);
	
	printf("PING %s\n", argv[1]);
	while(1) {
		status = sendto(sock, buffer, sizeBuf, 0, &addr, sizeAddr);
		
		recvfrom(sock, buffer, sizeBuf, 0, &addr, &sizeAddr);
		printf("Recv %s from %s\n", 
			buffer + sizeof(struct iphdr) + sizeof(struct icmphdr), 
			inet_ntoa(addr.sin_addr));
		sleep(1);
	}
	exit(EXIT_SUCCESS);
}
