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

#define MSGPERM 				0644    // msg queue permission
#define BUFF					256
#define CLIENT_TO_SERVER 		1
#define SERVER_TO_ALL_CLIENTS 	2

/* Global variables */
int msgQueue, rc;


int ReadMessage(int queue, Mesg* msg, long msg_type);

int SendMessage(int queue, Mesg* msg);

int RemoveQueue(int);

int main(int argc,char **argv);

int ParseCommandLine(void);

int OpenQueue(void);

int OpenNewQueue(void);

int Server(void);

int SearchForClients(void);

int Client(void);

int ProcessClient(Mesg* msg, int queue);

FILE* OpenFile(const char* fileName);

int PacketizeData(FILE* fp,
	              const int queue,
	              const long msg_type,
	              const int priority);

void sig_handler(int sig);

int DesignatePriority(const char* text,
                      char* name,
                      int* priority,
                      pid_t* client);
