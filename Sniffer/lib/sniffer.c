#include "../include/sniffer.h"

struct sockaddr_in source, dest;
int i = 0, j = 0, total = 0;

void processPacket(u_char *args, const struct pcap_pkthdr *header, const u_char *buffer) {
    int size = 0, sum = 0;
    unsigned short iphdrlen;
    struct iphdr *iph = NULL;
    
    iph = (struct iphdr *) (buffer  + sizeof(struct ethhdr));
    iphdrlen = iph->ihl * 4;
    size = header->len;
    total++;
	printIpHeader(buffer, size);
    PrintData(buffer, iphdrlen);
    sum = ntohs(iph->check);
    computeIpChecksum(iph);
    printf("Counted checksum: %d\n", sum);
    printf("Packets total: %d\n", total);
}

void printEthernetHeader(const u_char *Buffer, int Size) {
    struct ethhdr *eth = (struct ethhdr *)Buffer;
     
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
    printf("   |-Protocol            : %u \n", (unsigned short)eth->h_proto);
}
 
void printIpHeader(const u_char * Buffer, int Size) {
    struct iphdr *iph;
    
    printEthernetHeader(Buffer , Size);
     
    iph = (struct iphdr *) (Buffer + sizeof(struct ethhdr));
    memset(&source, 0, sizeof(source));
    source.sin_addr.s_addr = iph->saddr;
     
    memset(&dest, 0, sizeof(dest));
    dest.sin_addr.s_addr = iph->daddr;
     
    printf("\n");
    printf("IP Header\n");
    printf("   |-IP Version        : %d\n", (unsigned int)iph->version);
    printf("   |-IP Header Length  : %d DWORDS or %d Bytes\n", 
		(unsigned int)iph->ihl, ((unsigned int)(iph->ihl))*4);
    printf("   |-Type Of Service   : %d\n", (unsigned int)iph->tos);
    printf("   |-IP Total Length   : %d  Bytes(Size of Packet)\n", 
		ntohs(iph->tot_len));
    printf("   |-Identification    : %d\n",ntohs(iph->id));
    printf("   |-TTL      : %d\n", (unsigned int)iph->ttl);
    printf("   |-Protocol : %d\n", (unsigned int)iph->protocol);
    printf("   |-Checksum : %d\n", ntohs(iph->check));
    printf("   |-Source IP        : %s\n", inet_ntoa(source.sin_addr));
    printf("   |-Destination IP   : %s\n", inet_ntoa(dest.sin_addr));
}

void PrintData (const u_char * data , int Size) {
    int i , j;
    for(i = 0; i < Size; i++) {
        if(i != 0 && i % 16 == 0) {
            printf("         ");
            for(j = i - 16; j < i; j++) {
                if(data[j] >= 32 && data[j] <= 128)
                    printf("%c", (unsigned char)data[j]);
                else printf("."); 
            }
            printf("\n");
        } 
         
        if(i % 16 == 0) 
			printf("   ");
        printf(" %02X", (unsigned int)data[i]);
                 
        if(i == Size - 1) {
            for(j = 0; j < 15 - i % 16; j++) {
              printf("   ");
            }
             
            printf("         ");
             
            for(j = i - i % 16; j <= i; j++) {
                if(data[j] >= 32 && data[j] <= 128)  {
                  printf("%c", (unsigned char)data[j]);
                } else {
                  printf(".");
                }
            }
             
            printf("\n" );
        }
    }
}

void computeIpChecksum(struct iphdr* iphdrp){
  iphdrp->check = 0;
  iphdrp->check = checkSum((unsigned short*) iphdrp, iphdrp->ihl << 2);
}

int checkSum(unsigned short *addr, unsigned int count) {
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
