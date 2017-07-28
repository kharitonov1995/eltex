#include "../include/sniffer.h"

int main() {
	pcap_if_t *alldevsp, *device;
    pcap_t *handle;
    char errbuf[100], *devname, devs[100][100], *buf = NULL;
    int count = 1, n;
     
    printf("Finding available devices ... ");
    if(pcap_findalldevs(&alldevsp, errbuf)) {
        printf("Error finding devices : %s" , errbuf);
        exit(EXIT_FAILURE);
    }
    printf("Done");
    
    printf("\nAvailable Devices are :\n");
    for(device = alldevsp; device != NULL; device = device->next) {
        printf("%d. %s - %s\n", count, device->name, device->description);
        if(device->name != NULL) {
            strcpy(devs[count], device->name);
        }
        count++;
    }
    
    buf = calloc(3, sizeof(char));
    printf("Enter the number of the device you want to sniff: ");
    if (fgets(buf, 3, stdin) == NULL) {
		perror("fgets()");
		exit(EXIT_FAILURE);
	}
    n = atoi(buf);
    devname = devs[n];
    free(buf);
    printf("Opening device %s for sniffing ... " , devname);
    handle = pcap_open_live(devname, 65536, 1, 0, errbuf);
     
    if (handle == NULL) {
        fprintf(stderr, "Couldn't open device %s: %s\n", devname, errbuf);
        exit(1);
    }
    printf("Done\n");
     
    pcap_loop(handle, -1, processPacket, NULL);
    
	exit(EXIT_SUCCESS);
}
