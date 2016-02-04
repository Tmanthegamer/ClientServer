/* 
===============================================================================
SOURCE FILE:    Client.h 
                    Header file 

PROGRAM:        Client

FUNCTIONS:      int main(void)
                int Client(void)
                int PromptUserInput(char* input)
                int CreateReadThread(void)
                void* ReadServerResponse(void *queue)
                void sig_handler(int sig)


DATE:           January 30, 2016

REVISIONS:      January 31, 2016        (Tyler Trepanier-Bracken)
                    Splitting the old "ClientServer" functionality into
                    the Client files and the Server files. In addition, 
                    worked on Prompt-User Input for bug fixing and formatting
                    the desired message.
                
                Febuary 2, 2016         (Tyler Trepanier-Bracken)
                    Both Client and Server files have been split from each
                    other and any shared functionality has been included
                    inside of the Utilities files. NOTE: Both Server and
                    Client has their own definitions of the sig_handler
                    with their own implementations.

DESIGNGER:      Tyler Trepanier-Bracken

PROGRAMMER:     Tyler Trepanier-Bracken

NOTES:
Client is a stand-alone program which allows the user to ask for files to read
and displayed onto the stdout of this process. This client may have multiple
instances at once and they work in unison with only one Server. 

Each individual Client requests the user to enter in the filename and, if
they so please, the priority and another client's process id. This request
gets bundled into a message which gets sent to the server where the server
processes this file opening request. Afterwards, the server responds which
either an error message or the contents of the file.

This is the main file that holds all of the unique functionality of the Client
program. There are some shared functionality with the Server that is defined
inside of the Utilities files.
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

#define MSGPERM                 0644    // msg queue permission
#define BUFF                    256
#define CLIENT_TO_SERVER        100
#define SERVER_TO_ALL_CLIENTS   200

/* Global variables */
int msgQueue; 
int rc;

/*
===============================================================================
FUNCTION:       Read Message 

DATE:           January 28, 2016

REVISIONS:      January 30, 2016    (Tyler Trepanier-Bracken)
                    No longer hardcoding the values of the msg_type and
                    the message itself.
                Febuary 1, 2016     (Tyler Trepanier-Bracken)
                    Removed debug statements. 

DESIGNER:       Tyler Trepanier-Bracken

PROGRAMMER(S):  Tyler Trepanier-Bracken

INTERFACE:      int ReadMessage(int queue, 
                                Mesg* msg, 
                                long msg_type)

PARAMETERS:     int queue
                    Message queue to which all messages are sent to.
                Mesg* msg
                    Destination message structure which will be filled with
                    the accompanying message from the message queue.
                long msg_type
                    Type of message, used to differiante messages meant for
                    different processes.

RETURNS:        -Returns -1 on failure to read a message.      
                -Returns 0 on received message success.

NOTES:
Reads a message from an existing linux message queue. Uses the IPC_NOWAIT to
allow this function to immediately return -1 if there are no current messages
of a mentioned type in the queue.
===============================================================================
*/
int ReadMessage(int queue, Mesg* msg, long msg_type);

/*
===============================================================================
FUNCTION:       Send Message

DATE:           January 28, 2016

REVISIONS:      January 30, 2016    (Tyler Trepanier-Bracken)
                    No longer hardcoding the values of the msg_type and
                    the message itself.
                Febuary 1, 2016     (Tyler Trepanier-Bracken)
                    Inserted the message length calculation inside and 
                    removed all debug statements.

DESIGNER:       Tyler Trepanier-Bracken

PROGRAMMER(S):  Tyler Trepanier-Bracken

INTERFACE:      int SendMessage(int queue, Mesg* msg)

PARAMETERS:     Mesg* msg
                    Source message structure which will place its message contents
                    onto the message queue.
                long msg_type
                    Type of message, used to differiante messages meant for
                    different processes.

RETURNS:        -Returns -1 on failure to send a message.      
                -Returns 0 on received message success.

NOTES:
Sends a message from an existing linux message queue. Uses no flags to allow 
this function to wait on the message queue to free messages to be send if
there is an excess of messages already inside of the message queue.
===============================================================================
*/
int SendMessage(int queue, Mesg* msg);

/*
===============================================================================
FUNCTION:       Send Final Message 

DATE:           Feb 1, 2016

DESIGNER:       Tyler Trepanier-Bracken

PROGRAMMER(S):  Tyler Trepanier-Bracken

INTERFACE:      int SendFinalMessage(int queue, Mesg* msg)

PARAMETERS:     Mesg* msg
                    Source message structure which will place its message contents
                    onto the message queue.
                long msg_type
                    Type of message, used to differiante messages meant for
                    different processes.

RETURNS:        -Returns -1 on failure to send a message.      
                -Returns 0 on received message success.

NOTES:
Sends a message from an existing linux message queue. Makes use of the 
pre-existing SendMessage function to send an empty message to a client to 
CONFIRM a completed message.
===============================================================================
*/
int SendFinalMessage(int queue, Mesg* msg);

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
s
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
FUNCTION:       Remove Queue 

DATE:           January 28, 2016

DESIGNER:       Tyler Trepanier-Bracken

PROGRAMMER(S):  Tyler Trepanier-Bracken

INTERFACE:      int RemoveQueue(int queue)

PARAMETERS:     int queue
                    Message queue to be removed from the linux message.

RETURNS:        -Returns -1 if there is a failure to remove the message queue.
                Usually occurs if the message queue has been previously
                removed.          
                -Returns 0 on successful removal of the message queue.

NOTES:
Wrapper function that removes the message queue from the linux machine.
===============================================================================
*/
int RemoveQueue(int queue);

/*
===============================================================================
FUNCTION:       Open Queue 

DATE:           January 28, 2016

REVISIONS:      Febuary 1, 2016 (Tyler Trepanier-Bracken)
                    Removed the previous function OpenNewQueue and instead
                    used only the IPC_CREAT flag for opening an existing queue
                    or creating a new queue.

DESIGNER:       Tyler Trepanier-Bracken

PROGRAMMER(S):  Tyler Trepanier-Bracken

INTERFACE:      int OpenQueue(void)

PARAMETERS:     void

RETURNS:        -Returns -1 on failure to open or create a message queue.
                -Return 0 on successfully opening or creating a message queue.

NOTES:
Creates or opens a message queue which will allow interprocess communication
between the server and clients.
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
FILE* OpenFile(const char* fileName);
