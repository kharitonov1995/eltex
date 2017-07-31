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
    iph->ttl = 0;
    iph->protocol = IPPROTO_ICMP;
    iph->check = 0;
    iph->saddr = inet_addr(IP_ADDR);
    iph->daddr = ipAddrD;
        
    iph->check = checkSum((unsigned short*) iph, sizeof(struct iphdr));
    memcpy(buffer, iph, sizeof(struct iphdr));
    free(iph);
    return 0;
}

int addIcmpHeader(unsigned char *buffer, int pid) {
    struct icmphdr *icmp = NULL;
    static int sequence = 0;
    icmp = malloc(sizeof(struct icmphdr));
    
    icmp->type = ICMP_TIME_EXCEEDED;
    icmp->code = 0;
    icmp->un.echo.sequence = sequence++;
    icmp->un.echo.id = pid;
    icmp->checksum = checkSum((unsigned short*) icmp, sizeof(struct icmphdr));
    
    memcpy(buffer, icmp, sizeof(struct icmphdr));
    free(icmp);
    return 0;
}

int listener(int sock, uint32_t ipAddrD) {
    int pid = 0, status = 0, ttlCount = 0;
    struct iphdr *ip = NULL;
    struct icmphdr *icmp = NULL;
    struct sockaddr_in addr;
    socklen_t sizeAddr;
    unsigned char *buffer = NULL;
	struct pollfd *pollFd = NULL;
    struct in_addr *address = NULL;
    
    buffer = malloc(sizeof(char) * sizeBuf);
    ip = (struct iphdr *) buffer;
    icmp = (struct icmphdr *) buffer + sizeof(struct iphdr);
    pollFd = malloc(sizeof(struct pollfd));
    
    pollFd->fd = sock;
    pollFd->events = POLLIN;
    
    pid = getpid();
    sizeAddr = sizeof(addr);
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = ipAddrD;
    
    addIpHeader(sock, ipAddrD, buffer);
    addIcmpHeader(buffer + sizeof(struct iphdr), pid);
    
    while (1) {
        sendto(sock, buffer, sizeBuf, 0, &addr, sizeAddr);
        
        status = poll(pollFd, 1, 3000);
        if (status < 0) {
            perror("poll()");
            return -1;
        }
        if (!status) {
            address = (struct in_addr *) buffer + sizeof(struct iphdr) + sizeof(struct icmphdr);
            printf("%s\n", inet_ntoa(*address));
            ip->ttl = ++ttlCount;
        } else {
            recvfrom(sock, buffer, sizeBuf, 0, &addr, &sizeAddr);
                    
            printf("Recv %s from %s\n", 
                buffer + sizeof(struct iphdr) + sizeof(struct icmphdr), 
                inet_ntoa(addr.sin_addr));
            printf("IPv%d: hdr-size=%d pkt-size=%d protocol=%d TTL=%d src=%s\n",
                ip->version, ip->ihl*4, ntohs(ip->tot_len), ip->protocol,
                ip->ttl, inet_ntoa(addr.sin_addr));
            
            if (icmp->un.echo.id == pid) {
                printf("ICMP: type[%d/%d] checksum[%d] id[%d] seq[%d]\n",
                    icmp->type, icmp->code, ntohs(icmp->checksum),
                    icmp->un.echo.id, icmp->un.echo.sequence);
            }
        }
    }
    
    free(ip);
    free(icmp);
    free(buffer);
    free(pollFd);
    close(sock);
    
    return 0;
}

int pinger(int sock, uint32_t ipAddrD) {
    int status = 0, count = 5, pid = 0;
    struct sockaddr_in addr;
    unsigned char *buffer = NULL;
    socklen_t sizeAddr = 0;
    
    buffer = malloc(sizeof(char) * sizeBuf);
    
    status = addIpHeader(sock, ipAddrD, buffer);
    if (status < 0) {
        free(buffer);
        close(sock);
        return -1;
    }
    
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = ipAddrD;
    
    sizeAddr = sizeof(addr);
    pid = getppid();
    while (count > 0) {
        status = addIcmpHeader(buffer + sizeof(struct iphdr), pid);
        if (status < 0) {
            free(buffer);
            close(sock);
            return -1;
        }
       
        status = sendto(sock, buffer, sizeBuf, 0, &addr, sizeAddr);
         if (status < 0) {
            free(buffer);
            close(sock);
            return -1;
        }
        
        bzero(buffer + sizeof(struct iphdr), sizeof(struct icmphdr));
        count--;
        sleep(1);
    }
    
    free(buffer);
    close(sock);
    
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
