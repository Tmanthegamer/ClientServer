#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <mqueue.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/wait.h>
#include <sys/errno.h>


#include "mesg.h"

#define MSGPERM 0666    // msg queue permission

int ReadMessage(int queue, Mesg* msg, long msg_type);

int SendMessage(int queue, long type, char* data);

int RemoveQueue(int);

int main(int argc,char **argv);

int ParseCommandLine(void);

int Server(void);

int SearchForClients(void);

int CreateNewClient(void);

int ProcessClient(Mesg* msg, int priority, long PID, int queue);

FILE* OpenFile(const char* fileName);

int PacketizeData(FILE* fp, 
	              const int queue, 
	              const long msg_type, 
	              const int priority);
