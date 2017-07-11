#define _POSIX_C_SOURCE 1
#define _GNU_SOURCE
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <syslog.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/types.h>

#ifndef HANDLER_H
#define HANDLER_H
extern const char fifoInName[];
extern const char fifoOutName[];

pid_t initDaemon();
void mPerror(const char*);
pid_t createConnection(int*);
pid_t connectToServer(int*);
void processClient(int, int);
int processServer(int);
int listenFIFOServer(int);
char *listenFIFO(int);
char *getCurrentTime();
#endif
