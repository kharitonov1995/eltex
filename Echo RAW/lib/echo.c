#include "../include/echo.h"

int sizeBuf = 64;

int createServerUDP(struct sockaddr_in *addrServer) {
	int sock = 0;
	socklen_t size = sizeof(*addrServer);
	
	sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (sock < 0) {
		perror("socket()");
		return -1;
	}

	addrServer->sin_family = AF_INET;
	addrServer->sin_port = htons(PORT);
	addrServer->sin_addr.s_addr = htonl(INADDR_ANY);
	
	if (bind(sock, addrServer, size) < 0) {
		perror("bind()");
		return -1;
	}
	
	return sock;
}

int createSocketRaw(struct sockaddr_in *addrServer) {
	int sock = 0;
	
	sock = socket(AF_INET, SOCK_RAW, IPPROTO_UDP);
	if(sock < 0) {
		perror("socket()");
		return -1;
	}
	
	addrServer->sin_family = AF_INET; 
	addrServer->sin_port = htons(PORT); 
	addrServer->sin_addr.s_addr = inet_addr(IP_ADDR);
	
	return sock;
}

void addUdpHeader(unsigned char *buffer) {
	struct udphdr *udph = NULL;
	
	udph = malloc(sizeof(struct udphdr));
	
	bzero(udph, sizeof(struct udphdr));
		
	udph->source = htons(CLIENT_PORT);
	udph->dest = htons(PORT);
	udph->len = htons(sizeof(struct udphdr*) + sizeBuf);
	udph->check = 0;
	
	memcpy(buffer, udph, sizeof(struct udphdr));
	free(udph);
}

int addIpHeader(int sock, unsigned char *buffer) {
	int status = 0, flag = 1;
	struct iphdr *iph = NULL;
	
	if (sock < 0) {
		status = setsockopt(sock, IPPROTO_IP, IP_HDRINCL, &flag, sizeof(flag));
		if(status < 0) {
			perror("setsockopt()");
			return -1;
		}
	}
	
	iph = malloc(sizeof(struct iphdr));
	bzero(iph, sizeof(struct iphdr));
	
	iph->ihl = 5;
    iph->version = 4;
    iph->tos = 0;
    iph->tot_len = htons(sizeof(struct iphdr) + sizeof(struct udphdr) + sizeBuf);
    iph->id = htons(12345);
    iph->frag_off = 0;
    iph->ttl = 255;
    iph->protocol = IPPROTO_UDP;
    iph->check = 0;
    iph->saddr = inet_addr(IP_ADDR);
    iph->daddr = inet_addr(IP_ADDR);
		
	memcpy(buffer, iph, sizeof(struct iphdr));
	free(iph);
	return 0;
}

int clientRaw(int type) {
	int status = 0, sock = 0, totalSize = 0;
	struct sockaddr_in addrServer;
	socklen_t sizeAddr;
	unsigned char *buffer = NULL;
	struct udphdr *udph = NULL;
    struct iphdr *iph = NULL;
    
	sizeAddr = sizeof(addrServer);
	
	totalSize = sizeBuf + sizeof(struct udphdr) + sizeof(struct iphdr);
	buffer = calloc(totalSize, sizeof(unsigned char));
	iph = malloc(sizeof(struct iphdr));
	udph = malloc(sizeof(struct udphdr));
	
	if (type == 1) {
		sock = createSocketRaw(&addrServer);
		if (sock < 0) {
			return -1;
		}
		printf("Connected UDP\n");
		addUdpHeader(buffer);
		memcpy(buffer + sizeof(struct udphdr), "HELLO", strlen("HELLO"));
		if (status < 0) {
			perror("sendto()");
			return -1;
		}
	} else if (type == 2) {
		sock = createSocketRaw(&addrServer);
		if (sock < 0) {
			return -1;
		}
		printf("Connected UDP\n");
		addIpHeader(sock, buffer);
		addUdpHeader(buffer + sizeof(struct iphdr));
		memcpy(
			sizeof(struct iphdr) + sizeof(struct udphdr) + buffer, 
			"HELLO", 
			strlen("HELLO"));
	} else {
		printf("Incorrect type of client.\n");
		return -1;
	}
	status = sendto(
				sock,
				buffer,
				totalSize,
				0,
				&addrServer, 
				sizeAddr);
	if (status < 0) {
		perror("sendto()");
		return -1;
	}
	
	do {
		bzero(udph, sizeof(struct udphdr));
		bzero(iph, sizeof(struct iphdr));
		bzero(buffer, totalSize);
		
		status = recvfrom(
					sock, 
					buffer, 
					totalSize, 
					0, 
					&addrServer, 
					&sizeAddr);
		if (status < 0) {
			perror("recvfrom()");
			return -1;
		}
		
		memcpy(iph, buffer, sizeof(struct iphdr));
		memcpy(udph, buffer + sizeof(struct iphdr), sizeof(struct udphdr));
	} while(CLIENT_PORT != ntohs(udph->dest));
	
	printf("\n");
	printf("IP Header\n");
	printf("   |-IP Version        : %d\n", (unsigned int) iph->version);
	printf("   |-IP Header Length  : %d DWORDS or %d Bytes\n", 
		(unsigned int) iph->ihl, ((unsigned int) (iph->ihl)) * 4);
	printf("   |-Type Of Service   : %d\n", (unsigned int)iph->tos);
	printf("   |-IP Total Length   : %d  Bytes(Size of Packet)\n", 
		ntohs(iph->tot_len));
	printf("   |-Identification    : %d\n",ntohs(iph->id));
	printf("   |-TTL      : %d\n", (unsigned int) iph->ttl);
	printf("   |-Protocol : %d\n", (unsigned int) iph->protocol);
	printf("   |-Checksum : %d\n", ntohs(iph->check));
	
	printf("\nUDP Header\n");
	printf("   |-Source Port      : %d\n", ntohs(udph->source));
	printf("   |-Destination Port : %d\n", ntohs(udph->dest));
	printf("   |-UDP Length       : %d\n", ntohs(udph->len));
	printf("   |-UDP Checksum     : %d\n", ntohs(udph->check));
	
	printf("Recv %s from %s:%d \n", 
		buffer + sizeof(struct iphdr) + sizeof(struct udphdr), 
		inet_ntoa(addrServer.sin_addr), 
		ntohs(udph->source));
	
	free(udph);
	free(iph);
	free(buffer);
	close(sock);
	return 0;
}

int clientRawEthernet() {
	int sock = 0, status = 0, totalSize = 0;
	struct ethhdr *eh = NULL;
	struct sockaddr_ll ethernet;
    struct ifreq ifrIndx;
    unsigned char *buffer = NULL;
    unsigned char macWifi[6] = {0x74, 0xE5, 0x43, 0x47, 0x93, 0x27};
    /*unsigned char macEth[6] = {0xF0, 0xDE, 0xF1, 0xFE, 0xED, 0x58};*/
    unsigned char macLo[6] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
	char nameInterface[32];
    
	strncpy(nameInterface, LO_INTERFACE, strlen(LO_INTERFACE));
	
	sock = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
	if(sock < 0) {
		perror("socket()");
		return -1;
	}
	
	memset(&ifrIndx, 0, sizeof(struct ifreq));
	strncpy(ifrIndx.ifr_name, nameInterface, strlen(nameInterface));
	if (ioctl(sock, SIOCGIFINDEX, &ifrIndx) < 0) {
	    perror("ioctl(SIOCGIFINDEX)");
	    return -1;
	}
	
	status = setsockopt(
					sock,
					SOL_SOCKET,
					SO_BINDTODEVICE,
					&ifrIndx,
					sizeof(ifrIndx));
	if (status < 0)	{
		perror("setsockopt(SO_BINDTODEVICE)");
		close(sock);
		exit(EXIT_FAILURE);
	}
	
	totalSize = sizeBuf + sizeof(struct udphdr) 
				+ sizeof(struct iphdr) + sizeof(struct ethhdr);
	eh = malloc(sizeof(struct ethhdr));
	buffer = calloc(totalSize, sizeof(unsigned char)); 
		
	memcpy(eh->h_source, macLo, sizeof(macLo));
	memcpy(eh->h_dest, macWifi, sizeof(macWifi));
	eh->h_proto = htons(ETH_P_IP);
	
	memcpy(buffer, eh, sizeof(struct ethhdr));
	addIpHeader(-1, buffer + sizeof(struct ethhdr));
	addUdpHeader(buffer + sizeof(struct ethhdr) + sizeof(struct iphdr));
	memcpy(
		buffer + sizeof(struct ethhdr) + sizeof(struct iphdr) + sizeof(struct udphdr), 
		"HELLO", 
		strlen("HELLO"));
	
	ethernet.sll_family = AF_PACKET;	
	ethernet.sll_protocol = htons(ETH_P_IP);	
	ethernet.sll_ifindex = ifrIndx.ifr_ifindex;
	ethernet.sll_hatype = ARPHRD_ETHER;
	ethernet.sll_pkttype = PACKET_OTHERHOST;
	ethernet.sll_halen = ETH_ALEN;
	
	memcpy(ethernet.sll_addr, macWifi, sizeof(macWifi));
	ethernet.sll_addr[6]  = 0x00;
	ethernet.sll_addr[7]  = 0x00;
	
	status = sendto(
				sock,
				buffer,
				totalSize,
				0,
				(struct sockaddr*) &ethernet,
				sizeof(struct sockaddr_ll));
	
	if (status < 0) {
		perror("sendto()");
		return -1;
	}
	
	bzero(buffer, totalSize);
	status = recvfrom(
				sock, 
				buffer, 
				totalSize, 
				0, 
				NULL, 
				NULL);
	if (status < 0) {
		perror("recvfrom()");
		return -1;
	}
	
	printf("Recv %s\n", 
	buffer + sizeof(struct ethhdr) + sizeof(struct iphdr) + sizeof(struct udphdr));
	
	return sock;
}

int listenerUDP(int sock) {
	struct sockaddr_in client;
	int status = 0, sizeBuf = 64;
	char *buffer;
	socklen_t sizeAddr;
	
	sizeAddr = sizeof(client);
	buffer = calloc(sizeBuf, sizeof(char));
	while(1) {
		status = recvfrom(sock, buffer, sizeBuf, 0, &client, &sizeAddr);
		if (status < 0) {
			perror("recvfrom()");
			return -1;
		}
		
		printf("Recv %s from client %s:%d \n", 
			buffer, 
			inet_ntoa(client.sin_addr), 
			ntohs(client.sin_port));
		
		bzero(buffer, sizeBuf);
		memcpy(buffer, "HI!", strlen("HI!"));
		
		status = sendto(sock, buffer, sizeBuf, 0, &client, sizeAddr);
		if (status < 0) {
			perror("sendto()");
			return -1;
		}
	}
	close(sock);
	free(buffer);
	return 0;
}
