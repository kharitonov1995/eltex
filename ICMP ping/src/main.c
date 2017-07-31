#include "../include/icmp.h"

int main(int argc, char *argv[]) {
    int sock = 0;
    uint32_t ipAddrD = 0;
    
    if (argc < 2) {
        printf("Usage: ./mping host...\n");
        exit(EXIT_FAILURE);
    }
    
    sock = createSock();
    if (sock < 0) {
        exit(EXIT_FAILURE);
    }
    
    ipAddrD =  inet_addr(argv[1]);
    
    printf("PING %s\n", argv[1]);
    if (fork() == 0) {
        if (pinger(sock, ipAddrD) == 0) { 
            exit(EXIT_SUCCESS);
        } else {
            exit(EXIT_FAILURE);
        }
    } else {            
        if (listener(sock) < 0) {
            close(sock);
            exit(EXIT_FAILURE);
        }
    }
    
    exit(EXIT_SUCCESS);
}
