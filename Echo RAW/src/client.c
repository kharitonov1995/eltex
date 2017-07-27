#include "../include/echo.h"

int main(int argc, char *argv[]) {
	int status = 0, typeClient = 0;
	
	if (argc < 2) {
		printf("Please select type socket:\n1 - Raw UDP\n2 - Raw IP\n3 - Raw Ethernet\n");
		exit(EXIT_FAILURE);
	}
	
	typeClient = atoi(argv[1]);
	if (typeClient == 1 || typeClient == 2) {
		status = clientRaw(typeClient);
		if (status < 0) {
			exit(EXIT_FAILURE);			
		}
	} else if (typeClient == 3) {
		clientRawEthernet();
	}
	
	exit(EXIT_SUCCESS);
}
