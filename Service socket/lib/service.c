#include "../include/service.h"

int createServerUDP(struct sockaddr_in *addrServer, struct queueId queues[]) {
	int sock = 0, countThread = 10, i = 0;
	socklen_t size;
	pthread_t *threads = NULL;
	
	sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (sock < 0) {
		perror("socket()");
		return -1;
	}

	addrServer->sin_family = AF_INET;
	addrServer->sin_port = htons(PORT);
	addrServer->sin_addr.s_addr = htonl(INADDR_ANY);
	size = sizeof(*addrServer);
	
	if (bind(sock, addrServer, size) < 0) {
		perror("bind()");
		return -1;
	}
	
	if (queues != NULL) {
		threads = malloc(sizeof(pthread_t) * countThread);
		for (i = 0; i < countThread; i++) {
			pthread_create(&threads[i], NULL, listenerQueue, queues);
		}
	}
	
	return sock;
}

int createServerTCP(struct sockaddr_in *addrServer) {
	int sock = 0;
	socklen_t size = sizeof(*addrServer);
	
	sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
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
	
	if (listen(sock, 5) < 0) {
		perror("listen()");
		return -1;
	}
	
	return sock;
}

int connectToServerTCP(struct sockaddr_in *addrServer) {
	int sock;
	socklen_t addrSize;
	
	sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(sock < 0) {
		perror("socket()");
		return -1;
	}
	
	addrServer->sin_family = AF_INET; 
	addrServer->sin_port = htons(PORT); 
	addrServer->sin_addr.s_addr = inet_addr(IP_ADDR);
	addrSize = sizeof(*addrServer);
	
	if(connect(sock, addrServer, addrSize) < 0)  {
		perror("connect()");
		return -1;
	}
	
	return sock;
}

int connectToServerUDP(struct sockaddr_in *addrServer, char *name) {
	int sock = 0, sizeBuf = 32;
	socklen_t addrSize = 0;
	char *message;
	
	sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if(sock < 0) {
		perror("socket()");
		return -1;
	}
	
	addrServer->sin_family = AF_INET; 
	addrServer->sin_port = htons(PORT); 
	addrServer->sin_addr.s_addr = inet_addr(IP_ADDR);
	addrSize = sizeof(*addrServer);
	message = calloc(sizeBuf, sizeof(char));
	
	memcpy(message, name, strlen(name));
	if (sendto(sock, message, sizeBuf, 0 , addrServer, addrSize) < 0) {
		perror("sendto()");
		return -1;
	}
	free(message);
	return sock;
}

void *listenerQueue(void *argumets) {
	struct queueId *queues = (struct queueId *) argumets;
	struct msgServer msg;
	int sizeMessage;
	
	sizeMessage = ARRAY_SIZE(msg.message);
	while(1) {
		msgrcv(queues[0].msqId, &msg, sizeof(struct msgServer), PUT_CLIENT, 0);
		printf("Client with name: %s requested time. %s:%d\n", 
			msg.message, 
			inet_ntoa(msg.addrClient.sin_addr), 
			ntohs(msg.addrClient.sin_port));
			
		getCurrentTime(msg.message, sizeMessage);
		msg.type = GET_CLIENT;
		msgsnd(queues[1].msqId, &msg, sizeof(struct msgServer), 0);
	}
	
	pthread_exit(0);
}

int listenerUDP(struct queueId queues[], int sock) {
	struct sockaddr_in client;
	struct msgServer msg;
	int status = 0, sizeMessage = 0;
	char *buffer;
	socklen_t sizeAddr;
	
	sizeAddr = sizeof(client);
	sizeMessage = ARRAY_SIZE(msg.message);
	buffer = calloc(sizeMessage, sizeof(char));
	
	while(1) {
		status = recvfrom(
					sock, 
					buffer, 
					sizeMessage, 
					MSG_DONTWAIT, 
					&client, 
					&sizeAddr);
		if (status < 0 && errno != EWOULDBLOCK) {
			perror("recvfrom()");
			return -1;
		}
		if (errno != EWOULDBLOCK) {
			msg.type = PUT_CLIENT;
			msg.addrClient = client;
			snprintf(msg.message, sizeMessage, "%s", buffer);
			msgsnd(queues[0].msqId, &msg, sizeof(struct msgServer), 0);
		}
		msgrcv(
			queues[1].msqId, 
			&msg, 
			sizeof(struct msgServer), 
			GET_CLIENT, 
			IPC_NOWAIT);
		if (errno != ENOMSG) {
			status = sendto(
				sock,
				msg.message, 
				sizeMessage, 
				0, 
				&msg.addrClient, 
				sizeAddr);
			if (status < 0) {
				perror("sendto()");
				return -1;
			}
		}
		bzero(buffer, sizeMessage);
		errno = 0;
		usleep(1000);
	}
	
	close(sock);
	free(buffer);
	return 0;
}

int listenerTCP_UDP(int *sockets, int type) {
	struct sockaddr_in client;
	struct timeval tv;
	struct pollfd *pollFds = NULL;
	struct epoll_event *events = NULL, *newEvents = NULL;
	int socketClient = 0, status = 0, sizeBuffer = 32, i = 0, epollFd = 0;
	int maxFd = 0, count = 0, retval = 0, isUDP = 0, nEvents = 0;
	char *buffer;
	socklen_t sizeAddr;
	fd_set rdSet;
	const int maxEvents = 10;
	
	sizeAddr = sizeof(client);
	buffer = calloc(sizeBuffer, sizeof(char));
	maxFd = sockets[0] > sockets[1] ? sockets[0] : sockets[1]; 
	count = ARRAY_SIZE(sockets);
	
	while(1) {	
		if (type == 1) {	
			FD_ZERO(&rdSet);
			for (i = 0; i < count; i++) {
				FD_SET(sockets[i], &rdSet);
			}
			tv.tv_sec = 10;
			tv.tv_usec = 0;
			
			retval = select(maxFd + 1, &rdSet, NULL, NULL, &tv);
			if (retval < 0) {
				perror("select()");
				return -1;
			}
			if (!retval) {
				printf("No receive data in five seconds\n");
				break;
			} else {
				if (FD_ISSET(sockets[0], &rdSet)) {
					isUDP = 0;
				} else if (FD_ISSET(sockets[1], &rdSet)) {
					isUDP = 1;
				}
			}
		} else if (type == 2) {
			if (pollFds == NULL) {
				pollFds = malloc(sizeof(struct pollfd) * count);
				for (i = 0; i < count; i++) {
					pollFds[i].fd = sockets[i];
					pollFds[i].events = POLLIN;
				}
			}
			
			retval = poll(pollFds, count, 10000);
			if (retval < 0) {
				perror("poll()");
				return -1;
			}
			if (!retval) {
				printf("No receive data in five seconds\n");
				break;
			} else {
				if (pollFds[0].revents & POLLIN) {
					isUDP = 0;
				} else if (pollFds[1].revents & POLLIN) {
					isUDP = 1;
				}
			}
		} else if (type == 3) {
			if (epollFd == 0) {
				epollFd = epoll_create(count);
				events = malloc(sizeof(struct epoll_event) * count);
				newEvents = malloc(sizeof(struct epoll_event) * count);
				
				for (i = 0; i < count; i++) {
					events[i].data.fd = sockets[i];
					events[i].events = EPOLLIN;
					
					retval = epoll_ctl(
								epollFd, 
								EPOLL_CTL_ADD, 
								sockets[i], 
								&events[i]);
					if (retval < 0) {
						perror("epoll_ctl()");
						return -1;
					}
				}
			}
			
			nEvents = epoll_wait(epollFd, newEvents, maxEvents, 10000);
			if (nEvents < 0) {
				perror("epoll_wait()");
				return -1;
			}
			
			if (nEvents > 0) {
				for (i = 0; i < nEvents; i++) {
					if (newEvents[i].data.fd == sockets[0]) {
						isUDP = 0;
					} else if (newEvents[i].data.fd == sockets[1]) {
						isUDP = 1;
					}
				}
			} else {
				printf("No receive data in five seconds\n");
				break;
			}
		}
		
		if (!isUDP) {
			socketClient = accept(sockets[0], &client, &sizeAddr);
			if (socketClient < 0) {
				perror("accept()");
				return -1;
			}
			
			status = recv(socketClient, buffer, sizeBuffer, 0);
			if (status < 0) {
				perror("recv()");
				return -1;
			}
			
			printf("Client with name: %s from TCP requested time. %s:%d\n", 
					buffer, 
					inet_ntoa(client.sin_addr), 
					ntohs(client.sin_port));
			
			getCurrentTime(buffer, sizeBuffer);
			
			status = send(socketClient, buffer, sizeBuffer, 0);
			if (status < 0) {
				perror("send()");
				return -1;
			}
			close(socketClient);
		} else {
			status = recvfrom(
						sockets[1], 
						buffer, 
						sizeBuffer, 
						0, 
						&client, 
						&sizeAddr);
			if (status < 0) {
				perror("recvfrom()");
				return -1;
			}
			printf("Client with name: %s from UDP requested time. %s:%d\n", 
					buffer, 
					inet_ntoa(client.sin_addr), 
					ntohs(client.sin_port));
			
			getCurrentTime(buffer, sizeBuffer);
			status = sendto(
						sockets[1],
						buffer, 
						sizeBuffer, 
						0, 
						&client, 
						sizeAddr);
			if (status < 0) {
				perror("sendto()");
				return -1;
			}
		}
		bzero(buffer, sizeBuffer);
	}
	
	for (i = 0; i < count; i++) {
		close(sockets[i]);
	}
	if (epollFd != 0) {
		free(events);
		free(newEvents);
		close(epollFd);
	}
	if (pollFds != NULL) 
		free(pollFds);
	free(buffer);
	return 0;
}

int listenerTCP(int sock) {
	struct sockaddr_in client;
	int socketClient = 0, status = 0, sizeBuf = 32;
	char *buffer;
	pid_t pid;
	socklen_t sizeAddr;
	
	sizeAddr = sizeof(client);
	buffer = calloc(sizeBuf, sizeof(char));
	
	while(1) {
		socketClient = accept(sock, &client, &sizeAddr);
		if (socketClient < 0) {
			perror("accept()");
			return -1;
		}
		pid = fork();
		if (pid < 0) {
			perror("fork()");
			return -1;
		}
		if (pid == 0) {
			status = recv(socketClient, buffer, sizeBuf, 0);
			if (status < 0) {
				perror("recv()");
				return -1;
			}
			
			printf("Client with name: %s requested time. %s:%d\n", 
			buffer, 
			inet_ntoa(client.sin_addr), 
			ntohs(client.sin_port));
			
			getCurrentTime(buffer, sizeBuf);
			
			status = send(socketClient, buffer, sizeBuf, 0);
			if (status < 0) {
				perror("send()");
				return -1;
			}
			close(socketClient);
			free(buffer);
			exit(EXIT_SUCCESS);
		}
		close(socketClient);
	}
	close(sock);
	free(buffer);
	return 0;
}

void *threadClient(void *args) {
	struct msgServer *msg = (struct msgServer *) args;
	struct sockaddr_in addrServer;
	char *buffer;
	int sizeBuffer = 0, sock = 0;
	socklen_t sizeAddr;
	
	sizeBuffer = ARRAY_SIZE(msg->message);
	buffer = calloc(sizeBuffer, sizeof(char));
	sizeAddr = sizeof(addrServer);
	if (msg->type == 1) {
		sock = connectToServerUDP(&addrServer, msg->message);
		if (sock < 0) {
			exit(EXIT_FAILURE);
		}
		if (recvfrom(sock, buffer, sizeBuffer, 0, &addrServer, &sizeAddr) < 0) {
			exit(EXIT_FAILURE);
		}
	} else if (msg->type == 2) {
		sock = connectToServerTCP(&addrServer);
		
		if (sock < 0) {
			exit(EXIT_FAILURE);
		}
		if (send(sock, msg->message, strlen(msg->message), 0) < 0) {
			perror("send()");
			exit(EXIT_FAILURE);
		}
		if (recv(sock, buffer, sizeBuffer, 0) < 0) {
			perror("recv()");
			exit(EXIT_FAILURE);
		}
	}
 	
	printf("%s\n", buffer);
	free(buffer);
	pthread_exit(0);
}

void getCurrentTime(char *buf, const int size) {
	time_t rawTime;
	struct tm *timeInfo;

	time(&rawTime);
	timeInfo = localtime(&rawTime);
	strftime(buf, size, "%d.%m.%y %H:%M", timeInfo);
}
