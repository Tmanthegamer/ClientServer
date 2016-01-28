#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <mqueue.h>
#include <time.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/wait.h>
#include <sys/errno.h>

#include "mesg.h"

#define MSGPERM 0666    // msg queue permission

int ReadMessage(int, Mesg*, size_t, long, int);

int SendMessage(int, Mesg*, int, int);

int RemoveQueue(int);

int main(int argc,char **argv);

int ParseCommandLine(void);

int Server(void);

int SearchForClients(void);

int CreateNewClient(void);

int ProcessClient(void);

int DesignatePriority(void);

int OpenFile(void);

int PacketizeData(void);


