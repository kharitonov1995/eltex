#include "../include/sniffer.h"

struct sockaddr_in source, dest;
struct pseudoHead {
	__u32 saddr;
	__u32 daddr;
	__u8  zero;
	__u8 proto;
	__u16 length;
};

void processPacket(u_char *args, const struct pcap_pkthdr *header, const u_char *buffer) {
    int size = 0;
    static int total = 0;
    struct iphdr *iph = NULL;
    
    iph = (struct iphdr *) (buffer  + sizeof(struct ethhdr));
    size = header->len;
    
    switch (iph->protocol) {
        case 6:
			printTcpPacket(buffer, size);
		break;
		case 17:
			printUdpPacket(buffer, size);
		break;
    }
    total++;
    printf("Packets total: %d\n", total);
}

void printEthernetHeader(const u_char *buffer, int size) {
    struct ethhdr *eth = (struct ethhdr *) buffer;
     
    printf("\n");
    printf("Ethernet Header\n");
    printf("   |-Destination Address: %.2X-%.2X-%.2X-%.2X-%.2X-%.2X \n", 
			eth->h_dest[0], 
			eth->h_dest[1], 
			eth->h_dest[2], 
			eth->h_dest[3], 
			eth->h_dest[4], 
			eth->h_dest[5]);
    printf("   |-Source Address      : %.2X-%.2X-%.2X-%.2X-%.2X-%.2X \n", 
			eth->h_source[0], 
			eth->h_source[1], 
			eth->h_source[2],
			eth->h_source[3], 
			eth->h_source[4], 
			eth->h_source[5]);
    printf("   |-Protocol            : %u \n", (unsigned short) eth->h_proto);
}
 
void printIpHeader(const u_char * buffer, int size) {
    struct iphdr *iph;
    
    printEthernetHeader(buffer, size);
     
    iph = (struct iphdr *) (buffer + sizeof(struct ethhdr));
    bzero(&source, sizeof(struct sockaddr_in));
    source.sin_addr.s_addr = iph->saddr;
     
    bzero(&dest, sizeof(struct sockaddr_in));
    dest.sin_addr.s_addr = iph->daddr;
     
    printf("\n");
    printf("IP Header\n");
    printf("   |-IP Version        : %d\n", (unsigned int)iph->version);
    printf("   |-IP Header Length  : %d DWORDS or %d Bytes\n", 
		(unsigned int)iph->ihl, ((unsigned int)(iph->ihl)) * 4);
    printf("   |-Type Of Service   : %d\n", (unsigned int)iph->tos);
    printf("   |-IP Total Length   : %d  Bytes(Size of Packet)\n", 
		ntohs(iph->tot_len));
    printf("   |-Identification    : %d\n",ntohs(iph->id));
    printf("   |-TTL      : %d\n", (unsigned int)iph->ttl);
    printf("   |-Protocol : %d\n", (unsigned int)iph->protocol);
    printf("   |-Checksum : %d\n", ntohs(iph->check));
    printf("   |-Source IP        : %s\n", inet_ntoa(source.sin_addr));
    printf("   |-Destination IP   : %s\n", inet_ntoa(dest.sin_addr));
    
    computeIpChecksum(iph);
    printf("\nCounted checksum IP header: %d\n", ntohs(iph->check));
}

void printTcpPacket(const u_char * buffer, int size) {
    unsigned short iphdrlen;
    struct iphdr *iph;
    struct tcphdr *tcph;
    struct pseudoHead tcpPseudo;
    int totalLen = 0, headerSize = 0, totalTcpLen = 0;
    unsigned short *tcp;
    
    iph = (struct iphdr *) (buffer + sizeof(struct ethhdr));
    iphdrlen = iph->ihl * 4;
    totalLen = ntohs(iph->tot_len);
    
    tcph = (struct tcphdr*) (buffer + iphdrlen + sizeof(struct ethhdr));
    headerSize = sizeof(struct ethhdr) + iph->ihl * 4 + tcph->doff * 4;
	totalTcpLen = sizeof(tcpPseudo) + tcph->doff * 4 + size - headerSize;
			
	tcp = calloc(totalTcpLen, sizeof(unsigned short));
    
	bzero(&tcpPseudo, sizeof(struct pseudoHead));
	tcpPseudo.saddr = iph->saddr;
	tcpPseudo.daddr = iph->daddr;
	tcpPseudo.proto = iph->protocol;
	tcpPseudo.length = htons(totalLen - iphdrlen);
    
    printf("\n\n*****************TCP Packet******************\n");  
         
    printIpHeader(buffer, size);
         
    printf("\n");
    printf("TCP Header\n");
    printf("   |-Source Port      : %u\n", ntohs(tcph->source));
    printf("   |-Destination Port : %u\n", ntohs(tcph->dest));
    printf("   |-Sequence Number    : %u\n", ntohl(tcph->seq));
    printf("   |-Acknowledge Number : %u\n", ntohl(tcph->ack_seq));
    printf("   |-Header Length      : %d DWORDS or %d BYTES\n", 
			(unsigned int) tcph->doff, (unsigned int) tcph->doff * 4);
    printf("   |-Urgent Flag          : %d\n", (unsigned int)tcph->urg);
    printf("   |-Acknowledgement Flag : %d\n", (unsigned int)tcph->ack);
    printf("   |-Push Flag            : %d\n", (unsigned int)tcph->psh);
    printf("   |-Reset Flag           : %d\n", (unsigned int)tcph->rst);
    printf("   |-Synchronise Flag     : %d\n", (unsigned int)tcph->syn);
    printf("   |-Finish Flag          : %d\n", (unsigned int)tcph->fin);
    printf("   |-Window         : %d\n", ntohs(tcph->window));
    printf("   |-Checksum       : %d\n", ntohs(tcph->check));
    printf("   |-Urgent Pointer : %d\n", tcph->urg_ptr);
    printf("\n");

	tcph->check = 0;
    	
	memcpy((unsigned char*) tcp, &tcpPseudo, sizeof(tcpPseudo));
	memcpy((unsigned char*) tcp + sizeof(tcpPseudo), tcph, tcph->doff * 4);
	memcpy((unsigned char*) tcp + sizeof(tcpPseudo) + tcph->doff * 4,
			(unsigned char*) buffer + headerSize, size - headerSize);
    tcph->check = checkSum(tcp, totalTcpLen);
        
    printf("Counted TCP checksum header: %d\n\n", ntohs(tcph->check));
    /*
    printf("                        DATA Dump                       ");
    printf("\n");
        
    printf("IP Header\n");
    printData(Buffer, iphdrlen);
         
    printf("TCP Header\n");
    printData(Buffer + iphdrlen, tcph->doff * 4);
         
    printf("Data Payload\n");    
    printData(Buffer + header_size , Size - header_size );
    */
    printf("\n#####################################################\n");
    
    free(tcp);
}


void printUdpPacket(const u_char *buffer, int size) {
    unsigned short iphdrlen;
    struct iphdr *iph;
    struct udphdr *udph;
    struct pseudoHead udpPseudo;
	int totalUdpLen = 0;
    unsigned short *udp;
    
    iph = (struct iphdr *) (buffer +  sizeof(struct ethhdr));
    iphdrlen = iph->ihl * 4;
    udph = (struct udphdr*) (buffer + iphdrlen  + sizeof(struct ethhdr));
     
    printf("\n\n*******************UDP Packet*********************\n");
    
    printIpHeader(buffer, size);
     
    printf("\nUDP Header\n");
    printf("   |-Source Port      : %d\n" , ntohs(udph->source));
    printf("   |-Destination Port : %d\n" , ntohs(udph->dest));
    printf("   |-UDP Length       : %d\n" , ntohs(udph->len));
    printf("   |-UDP Checksum     : %d\n" , ntohs(udph->check));

	udph->check = 0;
	bzero(&udpPseudo, sizeof(struct pseudoHead));
	udpPseudo.saddr = iph->saddr;
	udpPseudo.daddr = iph->daddr;
	udpPseudo.proto = iph->protocol;
	udpPseudo.length = udph->len;
	
	totalUdpLen = sizeof(struct pseudoHead) + ntohs(udph->len);
	udp = malloc(sizeof(unsigned short) * totalUdpLen);
	
	memcpy(
		(unsigned char *) udp, 
		&udpPseudo, 
		sizeof(struct pseudoHead));
	memcpy(
		(unsigned char *) udp + sizeof(struct pseudoHead),
		(unsigned char *) udph, 
		ntohs(udph->len));
    
    udph->check = checkSum(udp, totalUdpLen);
    printf("\nCounted UDP checksum header: %d\n\n", ntohs(udph->check));
    
    /* 
    printf("\n");
    printf("IP Header\n");
    printData(Buffer , iphdrlen);
         
    printf("UDP Header\n");
    printData(Buffer+iphdrlen , sizeof udph);
         
    printf("Data Payload\n");    
    
    printData(Buffer + header_size , Size - header_size);
    */
    printf("\n#####################################################\n");
    
    free(udp);
}

void printData(const u_char * data , int Size) {
    int i, j;
    for (i = 0; i < Size; i++) {
        if (i != 0 && i % 16 == 0) {
            printf("         ");
            for (j = i - 16; j < i; j++) {
                if (data[j] >= 32 && data[j] <= 128)
                    printf("%c", (unsigned char)data[j]);
                else printf("."); 
            }
            printf("\n");
        } 
         
        if (i % 16 == 0) 
			printf("   ");
        printf(" %02X", (unsigned int)data[i]);
                 
        if (i == Size - 1) {
            for (j = 0; j < 15 - i % 16; j++) {
              printf("   ");
            }
             
            printf("         ");
             
            for (j = i - i % 16; j <= i; j++) {
                if (data[j] >= 32 && data[j] <= 128)  {
                  printf("%c", (unsigned char)data[j]);
                } else {
                  printf(".");
                }
            }
             
            printf("\n" );
        }
    }
}

void computeIpChecksum(struct iphdr *iphdrp){
	iphdrp->check = 0;
	iphdrp->check = checkSum((unsigned short*) iphdrp, iphdrp->ihl << 2);
}

unsigned short checkSum(unsigned short *addr, unsigned int count) {
    unsigned long sum = 0;

	while (count > 1) {
		sum += *addr++;
		count -= 2;
	}

	if(count > 0) {
		sum += ((*addr)&htons(0xFF00));
	}

	while (sum>>16) {
		sum = (sum & 0xffff) + (sum >> 16);
	}

	sum = ~sum;
	return ((unsigned short)sum);
}
