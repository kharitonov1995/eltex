#include "../include/multicast.h"

int createServerUDP(struct sockaddr_in *addrClient) {
	int sock = 0;
	
	sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (sock < 0) {
		perror("socket()");
		return -1;
	}
	
	addrClient->sin_family = AF_INET;
	addrClient->sin_port = htons(PORT);
	addrClient->sin_addr.s_addr = inet_addr(MULTICAST_IP_ADDR);
	
	return sock;
}

int connectToServerUDP(struct sockaddr_in *addrClient) {
	int sock = 0, status = 0;
	socklen_t addrSize = 0;
	struct ip_mreq req;
	
	sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if(sock < 0) {
		perror("socket()");
		return -1;
	}
	
	req.imr_multiaddr.s_addr = inet_addr(MULTICAST_IP_ADDR);
	req.imr_interface.s_addr = inet_addr(IP_ADDR);
	
	status = setsockopt(
				sock, 
				IPPROTO_IP, 
				IP_ADD_MEMBERSHIP, 
				&req,
				sizeof(req));
	
	if (status < 0) {
		perror("setsockopt()");
		return -1;
	}
	
	addrClient->sin_family = AF_INET;
	addrClient->sin_port = htons(PORT);
	addrClient->sin_addr.s_addr = htonl(INADDR_ANY);
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
