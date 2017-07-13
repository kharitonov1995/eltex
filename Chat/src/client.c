#include "../include/chat.h"

int main(int argc, char *argv[]) {
	char *nameClient = NULL;
	struct msgClient msg;
	int count = 3;
	struct queueId *queues;
	long type = 0L;
	
	queues = malloc(sizeof(struct queueId) * count);
	
	if (argc < 2) {
		printf("Please input name\n");
		exit(EXIT_FAILURE);
	}
	
	nameClient = argv[1];
	type = (long) getpid();
	printf("type = %ld\n", type);
	connectToServer(count, queues, nameClient, type);
	sprintf(msg.message, "Hello");
	msg.type = type;
	msgsnd(queues[1].msqId, &msg, sizeof(struct msgClient), IPC_NOWAIT);
	bzero(msg.message, strlen(msg.message));
	msgrcv(queues[2].msqId, &msg, sizeof(struct msgClient), 0L, 0);
	printf("принято = %s\n", msg.message);
	getchar();
	disconnectClient(queues, nameClient, type);
	/*
	struct msqid_ds qstatus;
	
	printf("type = %ld\n", msg.type);
	if(msgctl(queues[1].msqId,IPC_STAT,&qstatus)<0){
	perror("msgctl failed");
	exit(1);
	}
	printf("Message queue id: %d\n", queues[1].msqId);
	printf("%d message(s) on queue\n",qstatus.msg_qnum);
	printf("Last message sent by process :%3d at %s \n",qstatus.msg_lspid,ctime(& (qstatus.msg_stime)));

	printf("Last message received by process :%3d at %s \n",qstatus.msg_lrpid,ctime(& (qstatus.msg_rtime)));
	printf("Current number of bytes on queue %d\n",qstatus.msg_cbytes);
	printf("Maximum number of bytes allowed on the queue%d\n",qstatus.msg_qbytes);*/
	/*printf("type = %ld\n", msg.type);
	printf("id = %d\n", queues[1].msqId);
	printf("id = %d\n", queues[2].msqId);*/
	/*bzero(msg.message, strlen(msg.message));
	msgrcv(queues[2].msqId, &msg, sizeof(struct msgClient), type, 0);
	
	printf("принято = %s\n", msg.message);*/
	/*key_t key;
	int msqId;
	const char *fileName = "./server1"; 
	key = ftok(fileName, '1');
	msqId = msgget(key, 0);
	msg = initList((int) getpid(), nameClient);
	getchar();
	memcpy(msg->message, "Hello", strlen("Hello"));
	msgsnd(msqId, msg, sizeof(List), 0);
	printf("type = %ld\n", msg->type);
	printf("id = %d\n", msqId);
	 
	msgrcv(msqId, &msgRcv, sizeof(List), msg->type, 0); 
	printf("имя: %s\n", msgRcv.name);
	printf("Сообщение: %s\n", msgRcv.message);
	printf("клиент: %s\n", msg->message);*/
	/*msgrcv(msqId, &msg, sizeof(struct msgServer), book.type, 0); 
	printf("message = %s\n", book.str);*/
	exit(EXIT_SUCCESS);
}
