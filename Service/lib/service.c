#include "../include/service.h"

const char fifoServerName[] = "/home/artem/Документы/eltex/Service/fifoServer";
const char errFile[] = "/home/artem/Документы/eltex/Service/errors";

void mPerror(const char *message) {
	if (message == NULL) {
		fprintf(stderr, "%s\n", strerror(errno));
	} else {
		fprintf(stderr, "%s: %s\n", message, strerror(errno));
	}
	fflush(stderr);
	sync();
}

pid_t initDaemon() {
	pid_t pid;
	int i, numFiles, stdio, mError;
	
	pid = fork();
	if (pid < 0) {
		perror("fork()");
		return -1;
	}
	if (pid > 0)
		exit(EXIT_SUCCESS);
	
	umask(0);
	if (setsid() < 0) {
		perror("setsid()");
		return -1;
	}
	if (chdir("/") < 0) {
		perror("chdir()");
		return -1;
	}
	
	stdio = open("dev/null", O_RDWR);
	if (stdio < 0) {
		perror("open()");
		return -1;
	}
	mError = open(errFile, O_APPEND | O_CREAT | O_WRONLY, 0666);
	if (mError < 0) {
		perror("open()");
		return -1;
	}
	
	dup2(stdio, STDOUT_FILENO);
	dup2(stdio, STDIN_FILENO);
	dup2(mError, STDERR_FILENO);
	
	numFiles = sysconf(_SC_OPEN_MAX);
	for(i = numFiles - 1; i >= 0; i--) {
		if (i != STDOUT_FILENO && i != STDERR_FILENO && STDIN_FILENO)
			close(i);
	}
	
	pid = getpid();
	
	syslog(LOG_INFO, "Daemon is created");
	return pid;
}

pid_t createConnection(int *fifoServer) {
	pid_t pidServer = 0;
	
	*fifoServer = mkfifo(fifoServerName, 0666);
	if(*fifoServer < 0 && errno != EEXIST) {
		mPerror("fifoServer mkfifo()");
		return -1;
	}
	
	*fifoServer = open(fifoServerName, O_RDWR);
	if(*fifoServer < 0) {
		mPerror("fifoServer open()");
		return -1;
	}
	
	pidServer = getpid();
	if (write(*fifoServer, &pidServer, sizeof(pid_t)) < 0) {
		mPerror("server write()");
		return -1;
	}
	
	syslog(LOG_INFO, "Connection is created");
	
	return 0;
}

pid_t connectToServer(int *fifoServer) {
	pid_t pidServer = 0;/*, pidClient = 0;*/
	
	*fifoServer = open(fifoServerName, O_RDWR);
	if (*fifoServer < 0) {
		perror("open()");
		return -1;
	}
	
	if (read(*fifoServer, &pidServer, sizeof(int)) < 0) {
		perror("client read()");
		return -1;
	}
	
	return pidServer;
}

void processClient(int pidServer, int fifoServer) {
	int ch = 0;
	char *time = NULL;
	
	 while(ch != 113 && ch != 107) {
		if (system("clear") < 0) {
			perror("system()");
			exit(EXIT_FAILURE);
		}
		fflush(stdout);
		printf("Press enter to watch system time...\n");
		printf("Press q to exit...\n");
		printf("Press k to kill daemon...\n");
		printf("> ");
		
		ch = fgetc(stdin);
		switch(ch) {
			case 10:
				kill(pidServer, SIGUSR1);
				time = listenFIFO(fifoServer);
				if (time == NULL) {
					printf("Server is not request\n");
					exit(EXIT_FAILURE);
				}
				printf("time: %s\n", time);
				memset(time, 0, strlen(time));
				break;
			case 107:
				kill(pidServer, SIGTERM);
				break;
			case 113:
				kill(pidServer, SIGUSR2);
				break;
		}
		printf("Press enter to continue...\n");
		fgetc(stdin);
	}
	if (time != NULL)
		free(time);
}

int processServer(int fifoServer) {
	int sign = 0;
	const int sizeBuf = 16;
	char *time;
	sigset_t set;
		
	time = malloc(sizeof(char) * sizeBuf);
	sigemptyset(&set);
	sigaddset(&set, SIGUSR1);
	sigaddset(&set, SIGUSR2);
	sigaddset(&set, SIGTERM);
	sigprocmask(SIG_BLOCK, &set, NULL);
	
	do {
		sigwait(&set, &sign);
		switch(sign) {
			case 10:
				time = getCurrentTime();
				if (write(fifoServer, time, strlen(time)) < 0) {
					mPerror("write()");
					return -1;
				}
				memset(time, 0, strlen(time));
			break;
			case 12:
				syslog(LOG_INFO, "Client is disconnected");
			break;
		}
	} while (sign != SIGTERM);
	
	free(time);
	close(fifoServer);
	unlink(fifoServerName);
	return 0;
}

char *listenFIFO(int fifoServer) {
	char *result;
	const int sizeBuf = 16;
	
	result = calloc(sizeBuf, sizeof(char));
	if (result == NULL) {
		perror("calloc()");
		return NULL;
	}
	
	if (read(fifoServer, result, sizeBuf) < 0) {
		mPerror("read()");
		return NULL;
	}
	return result;
}

char *getCurrentTime() {
	time_t rawtime;
	struct tm *timeinfo;
	int sizeBuf = 16;
	char *buf = calloc(sizeBuf, sizeof(char));

	time(&rawtime);
	timeinfo = localtime(&rawtime);
	strftime(buf, sizeBuf, "%d.%m.%y %H:%M", timeinfo);
	
	return buf;
}
