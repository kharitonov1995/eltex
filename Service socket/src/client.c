#include "../include/service.h"

int main(int argc, char *argv[]) {
	struct msgServer msg;
	int sizeMessage, i, count = 1000, typeServer = 0;
	char *nameClient = NULL;
	char *buffer = NULL;
	pthread_t *threads = NULL;
	
	if (argc < 3) {
		printf("Please input name and type server:\n1 - UDP\n2 - TCP");
		exit(EXIT_FAILURE);
	}
	
	nameClient = argv[1];
	typeServer = atoi(argv[2]);
	sizeMessage = ARRAY_SIZE(msg.message);
	buffer = calloc(sizeMessage, sizeof(char));
	threads = malloc(sizeof(pthread_t) * count);
	snprintf(msg.message, sizeMessage, "%s", nameClient);
	msg.type = typeServer;
	for (i = 0; i < count; i++) {
		pthread_create(&threads[i], NULL, threadClient, &msg);
	}
	
	for (i = 0; i < count; i++) {
		pthread_join(threads[i], NULL);
	}
	
	free(threads);
	free(buffer);
	exit(EXIT_SUCCESS);
}
