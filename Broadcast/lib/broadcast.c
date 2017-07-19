#include "../include/broadcast.h"

int createServerUDP(struct sockaddr_in *addrClient) {
	int sock = 0, status = 0;
	const int bcastOn = 1;
	
	sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (sock < 0) {
		perror("socket()");
		return -1;
	}
	
	status = setsockopt(
				sock, 
				SOL_SOCKET, 
				SO_BROADCAST, 
				&bcastOn, 
				sizeof(bcastOn));
	if (status < 0) {
		perror("setsokcopt()");
		return -1;
	}
	
	addrClient->sin_family = AF_INET;
	addrClient->sin_port = htons(PORT);
	addrClient->sin_addr.s_addr = inet_addr(BCAST_IP_ADDR);
		
	return sock;
}

int connectToServerUDP(struct sockaddr_in *addrClient) {
	int sock = 0;
	socklen_t addrSize = 0;
	
	sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if(sock < 0) {
		perror("socket()");
		return -1;
	}
	
	addrClient->sin_family = AF_INET;
	addrClient->sin_port = htons(PORT);
	addrClient->sin_addr.s_addr = inet_addr(BCAST_IP_ADDR);
	addrSize = sizeof(*addrClient);
	
	if (bind(sock, addrClient, addrSize) < 0) {
		perror("bind()");
		return -1;
	}
	
	return sock;
}

int listenerUDP(int sock) {
	close(sock);
	return 0;
}

void getCurrentTime(char *buf, const int size) {
	time_t rawtime;
	struct tm *timeinfo;

	time(&rawtime);
	timeinfo = localtime(&rawtime);
	strftime(buf, size, "%d.%m.%y %H:%M:%S", timeinfo);
}
