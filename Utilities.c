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

#include "Utilities.h"

int ReadMessage(int queue, Mesg* msg, long msg_type)
{
    rc = msgrcv(queue,msg,sizeof(msg->mesg_data),msg_type,0);
    if(rc < 0)
    {
        return -1;
    }

    return 0;
}

int SendMessage(int queue, Mesg* msg)
{
    msg->mesg_len = strlen(msg->mesg_data);
    
    /* This will keep trying to send messages the message queue if there are 
        too many messages in the queue. */
    rc = msgsnd(queue, msg, sizeof(msg->mesg_data), 0);

    if (rc < 0)
    {
        return -1;
    }

    return 0;
}

int SendFinalMessage(int queue, Mesg* msg)
{
    msg->mesg_data[0] = '\0';
    msg->mesg_data[1] = '\0';
    return SendMessage(queue, msg);
}

int RemoveQueue(int queue)
{
    int rc;
    if (( rc= msgctl(queue, IPC_RMID, NULL) ) < 0)
    {
        return 1;
    }
    return 0;
}

int OpenQueue(void)
{
    key_t key;

    key = ftok("Info", 'a');
    msgQueue = msgget(key, 0644|IPC_CREAT);

    if (msgQueue < 0) {
        return -1;
    }

    return 0;
}

FILE* OpenFile(const char* fileName)
{
    FILE *fp;

    fp = fopen(fileName, "r"); // read file

    return fp;
}
