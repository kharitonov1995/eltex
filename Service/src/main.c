#include "../include/service.h"

int main() {
	pid_t pid = 0, pidClient = 0;
	int fifoServer = 0;
	
	pid = initDaemon();
	
	openlog("service-daemon", LOG_PID, LOG_USER);
	if (pid < 0) {
		syslog(LOG_INFO, "Deamon create error");
		closelog();
		exit(EXIT_FAILURE);
	}
	
	pidClient = createConnection(&fifoServer);
	if (pidClient < 0) {
		exit(EXIT_FAILURE);
	}
	if (processServer(fifoServer) < 0) {
		syslog(LOG_INFO, "Server is shoutdown with error");
	}
	
	syslog(LOG_INFO, "Server is shoutdown normally");
	closelog();
	exit(EXIT_SUCCESS);
}
