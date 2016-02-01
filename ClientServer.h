/* 
===============================================================================
SOURCE FILE:    Client.h 
                    Header file 

PROGRAM:        Client Server

FUNCTIONS:      void* OutputFunction(void *message)
				int DisplayMessage(const char* message)
				int PromptUserInput(char* input)
				int ReadInput(void)
				int ReadServerResponse(void)
				int TerminateClient(void)


DATE:           January 9, 2016

REVISIONS:

DESIGNGER:      Tyler Trepanier-Bracken

PROGRAMMER:     Tyler Trepanier-Bracken

NOTES:
Standard Notes go here.
===============================================================================
*/

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
int msgQueue; 
int rc;

/*
===============================================================================
FUNCTION:       Main 

DATE:           January 9, 2016

REVISIONS:      (Date and Description)

DESIGNER:       Tyler Trepanier-Bracken

PROGRAMMER(S):  Tyler Trepanier-Bracken
                Harvey Dent

INTERFACE:      int main (char *process)

PARAMETERS:     char *process: 
                    the name of the process to be validated. 

RETURNS:        -Returns the PID of process specified if the process
                exists.          
                -Returns 0 if the process was not found in the process table.

NOTES:
Standard Notes go here. 
===============================================================================
*/
int ReadMessage(int queue, Mesg* msg, long msg_type);

/*
===============================================================================
FUNCTION:       Main 

DATE:           January 9, 2016

REVISIONS:      (Date and Description)

DESIGNER:       Tyler Trepanier-Bracken

PROGRAMMER(S):  Tyler Trepanier-Bracken
                Harvey Dent

INTERFACE:      int main (char *process)

PARAMETERS:     char *process: 
                    the name of the process to be validated. 

RETURNS:        -Returns the PID of process specified if the process
                exists.          
                -Returns 0 if the process was not found in the process table.

NOTES:
Standard Notes go here. 
===============================================================================
*/
int SendMessage(int queue, Mesg* msg);

/*
===============================================================================
FUNCTION:       Main 

DATE:           January 9, 2016

REVISIONS:      (Date and Description)

DESIGNER:       Tyler Trepanier-Bracken

PROGRAMMER(S):  Tyler Trepanier-Bracken
                Harvey Dent

INTERFACE:      int main (char *process)

PARAMETERS:     char *process: 
                    the name of the process to be validated. 

RETURNS:        -Returns the PID of process specified if the process
                exists.          
                -Returns 0 if the process was not found in the process table.

NOTES:
Standard Notes go here. 
===============================================================================
*/
int ReadAllMessages(int queue, Mesg* msg, long msg_type);

/*
===============================================================================
FUNCTION:       Main 

DATE:           January 9, 2016

REVISIONS:      (Date and Description)

DESIGNER:       Tyler Trepanier-Bracken

PROGRAMMER(S):  Tyler Trepanier-Bracken
                Harvey Dent

INTERFACE:      int main (char *process)

PARAMETERS:     char *process: 
                    the name of the process to be validated. 

RETURNS:        -Returns the PID of process specified if the process
                exists.          
                -Returns 0 if the process was not found in the process table.

NOTES:
Standard Notes go here. 
===============================================================================
*/
int RemoveQueue(int);

/*
===============================================================================
FUNCTION:       Main 

DATE:           January 9, 2016

REVISIONS:      (Date and Description)

DESIGNER:       Tyler Trepanier-Bracken

PROGRAMMER(S):  Tyler Trepanier-Bracken
                Harvey Dent

INTERFACE:      int main (char *process)

PARAMETERS:     char *process: 
                    the name of the process to be validated. 

RETURNS:        -Returns the PID of process specified if the process
                exists.          
                -Returns 0 if the process was not found in the process table.

NOTES:
Standard Notes go here. 
===============================================================================
*/
int main(int argc,char **argv);

/*
===============================================================================
FUNCTION:       Main 

DATE:           January 9, 2016

REVISIONS:      (Date and Description)

DESIGNER:       Tyler Trepanier-Bracken

PROGRAMMER(S):  Tyler Trepanier-Bracken
                Harvey Dent

INTERFACE:      int main (char *process)

PARAMETERS:     char *process: 
                    the name of the process to be validated. 

RETURNS:        -Returns the PID of process specified if the process
                exists.          
                -Returns 0 if the process was not found in the process table.

NOTES:
Standard Notes go here. 
===============================================================================
*/
int ParseCommandLine(void);

/*
===============================================================================
FUNCTION:       Main 

DATE:           January 9, 2016

REVISIONS:      (Date and Description)

DESIGNER:       Tyler Trepanier-Bracken

PROGRAMMER(S):  Tyler Trepanier-Bracken
                Harvey Dent

INTERFACE:      int main (char *process)

PARAMETERS:     char *process: 
                    the name of the process to be validated. 

RETURNS:        -Returns the PID of process specified if the process
                exists.          
                -Returns 0 if the process was not found in the process table.

NOTES:
Standard Notes go here. 
===============================================================================
*/
int OpenQueue(void);

/*
===============================================================================
FUNCTION:       Main 

DATE:           January 9, 2016

REVISIONS:      (Date and Description)

DESIGNER:       Tyler Trepanier-Bracken

PROGRAMMER(S):  Tyler Trepanier-Bracken
                Harvey Dent

INTERFACE:      int main (char *process)

PARAMETERS:     char *process: 
                    the name of the process to be validated. 

RETURNS:        -Returns the PID of process specified if the process
                exists.          
                -Returns 0 if the process was not found in the process table.

NOTES:
Standard Notes go here. 
===============================================================================
*/
int OpenNewQueue(void);

/*
===============================================================================
FUNCTION:       Main 

DATE:           January 9, 2016

REVISIONS:      (Date and Description)

DESIGNER:       Tyler Trepanier-Bracken

PROGRAMMER(S):  Tyler Trepanier-Bracken
                Harvey Dent

INTERFACE:      int main (char *process)

PARAMETERS:     char *process: 
                    the name of the process to be validated. 

RETURNS:        -Returns the PID of process specified if the process
                exists.          
                -Returns 0 if the process was not found in the process table.

NOTES:
Standard Notes go here. 
===============================================================================
*/
int Server(void);

/*
===============================================================================
FUNCTION:       Main 

DATE:           January 9, 2016

REVISIONS:      (Date and Description)

DESIGNER:       Tyler Trepanier-Bracken

PROGRAMMER(S):  Tyler Trepanier-Bracken
                Harvey Dent

INTERFACE:      int main (char *process)

PARAMETERS:     char *process: 
                    the name of the process to be validated. 

RETURNS:        -Returns the PID of process specified if the process
                exists.          
                -Returns 0 if the process was not found in the process table.

NOTES:
Standard Notes go here. 
===============================================================================
*/
int SearchForClients(void);

/*
===============================================================================
FUNCTION:       Main 

DATE:           January 9, 2016

REVISIONS:      (Date and Description)

DESIGNER:       Tyler Trepanier-Bracken

PROGRAMMER(S):  Tyler Trepanier-Bracken
                Harvey Dent

INTERFACE:      int main (char *process)

PARAMETERS:     char *process: 
                    the name of the process to be validated. 

RETURNS:        -Returns the PID of process specified if the process
                exists.          
                -Returns 0 if the process was not found in the process table.

NOTES:
Standard Notes go here. 
===============================================================================
*/
int Client(void);

/*
===============================================================================
FUNCTION:       Main 

DATE:           January 9, 2016

REVISIONS:      (Date and Description)

DESIGNER:       Tyler Trepanier-Bracken

PROGRAMMER(S):  Tyler Trepanier-Bracken
                Harvey Dent

INTERFACE:      int main (char *process)

PARAMETERS:     char *process: 
                    the name of the process to be validated. 

RETURNS:        -Returns the PID of process specified if the process
                exists.          
                -Returns 0 if the process was not found in the process table.

NOTES:
Standard Notes go here. 
===============================================================================
*/
int ProcessClient(Mesg* msg, int queue);

/*
===============================================================================
FUNCTION:       Main 

DATE:           January 9, 2016

REVISIONS:      (Date and Description)

DESIGNER:       Tyler Trepanier-Bracken

PROGRAMMER(S):  Tyler Trepanier-Bracken
                Harvey Dent

INTERFACE:      int main (char *process)

PARAMETERS:     char *process: 
                    the name of the process to be validated. 

RETURNS:        -Returns the PID of process specified if the process
                exists.          
                -Returns 0 if the process was not found in the process table.

NOTES:
Standard Notes go here. 
===============================================================================
*/
FILE* OpenFile(const char* fileName);

/*
===============================================================================
FUNCTION:       Main 

DATE:           January 9, 2016

REVISIONS:      (Date and Description)

DESIGNER:       Tyler Trepanier-Bracken

PROGRAMMER(S):  Tyler Trepanier-Bracken
                Harvey Dent

INTERFACE:      int main (char *process)

PARAMETERS:     char *process: 
                    the name of the process to be validated. 

RETURNS:        -Returns the PID of process specified if the process
                exists.          
                -Returns 0 if the process was not found in the process table.

NOTES:
Standard Notes go here. 
===============================================================================
*/
int PacketizeData(FILE* fp,
	              const int queue,
	              const long msg_type,
	              const int priority);

/*
===============================================================================
FUNCTION:       Main 

DATE:           January 9, 2016

REVISIONS:      (Date and Description)

DESIGNER:       Tyler Trepanier-Bracken

PROGRAMMER(S):  Tyler Trepanier-Bracken
                Harvey Dent

INTERFACE:      int main (char *process)

PARAMETERS:     char *process: 
                    the name of the process to be validated. 

RETURNS:        -Returns the PID of process specified if the process
                exists.          
                -Returns 0 if the process was not found in the process table.

NOTES:
Standard Notes go here. 
===============================================================================
*/
void sig_handler(int sig);

/*
===============================================================================
FUNCTION:       Main 

DATE:           January 9, 2016

REVISIONS:      (Date and Description)

DESIGNER:       Tyler Trepanier-Bracken

PROGRAMMER(S):  Tyler Trepanier-Bracken
                Harvey Dent

INTERFACE:      int main (char *process)

PARAMETERS:     char *process: 
                    the name of the process to be validated. 

RETURNS:        -Returns the PID of process specified if the process
                exists.          
                -Returns 0 if the process was not found in the process table.

NOTES:
Standard Notes go here. 
===============================================================================
*/
int DesignatePriority(const char* text,
                      char* name,
                      int* priority,
                      pid_t* client);
