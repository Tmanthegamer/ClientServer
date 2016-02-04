/* 
===============================================================================
SOURCE FILE:    Server.c
                    Definition file for the Server

PROGRAM:        Server

FUNCTIONS:      int main(void)
                int Server(void)
                int SearchForClients(void)
                int ProcessClient(Mesg* msg, int queue)
                int DesignatePriority(const char* text,
                      char* name,
                      int* priority,
                      pid_t* client)
                int PacketizeData(FILE* fp,
                      const int queue,
                      const long msg_type,
                      const int priority)
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
Server is a stand-alone program which allows any incoming Clients to request 
files to be read and its information sent to the client. There should be only 
one instance of Server as both Servers can cause collisions with each other.

The Server uses the client's filename, mentioned in the mesg_data, to open
a new file for reading. Upon success, it will disperse the contents of the
file to the Client. Failure will simply send a small message to the Client
indicating failure to open the file.

In addition, the Client can specify their priority level (defaults to max),
which will affect the speed of transmission between the Client and the Server.

This is the main file that holds all of the unique functionality of the Server
program. There are some shared functionality with the Client that is defined
inside of the Utilities files.
===============================================================================
*/
#include "Server.h"

Mesg* kill_client_msg;

extern int errno;       // error NO.
int quit = 0;

int main(void)
{
    
    sa.sa_handler = sig_handler;
    sigemptyset (&sa.sa_mask);
    sa.sa_flags = 0;

    sigaction (SIGINT, &sa, &oldint);
    sigaction (SIGTSTP, &sa, NULL);

    Server();

    // Restore normal action
    sigaction (SIGINT, &oldint, NULL);

    return 0;
}

int Server(void)
{

    if(OpenQueue() < 0)
        return 1;

    SearchForClients();

    RemoveQueue(msgQueue);
    return 0;
}

int SearchForClients(void)
{
    Mesg rcv;

    rcv.mesg_type = CLIENT_TO_SERVER;
    sprintf(rcv.mesg_data, "   ");
    kill_client_msg = &rcv;

    while (!quit){
        if(ReadMessage(msgQueue, &rcv, CLIENT_TO_SERVER) == 0)
        {
            pid_t child;
            child = fork();
            
            switch(child)
            {
            case -1:
                printf("Fatal error.\n");
                break;
            case 0: //child
                kill_client_msg = &rcv;
                ProcessClient(&rcv, msgQueue);
                exit(1);
                break;

            default: //parent
                break;
            }

        }

    }

    return 0;
}

int ProcessClient(Mesg* msg, int queue)
{
    FILE* file;
    char name[BUFF];
    pid_t client;
    int priority;

    if(DesignatePriority(msg->mesg_data, name, &priority, &client) < 0)
    {
        printf("Fatal error, cannot read message.");
        return -1;
    }

    if((file = OpenFile(name)) == NULL)
    {
        msg->mesg_type = client;
        sprintf(msg->mesg_data, "Cannot open file: %s\n", name);
       if(SendMessage(queue, msg) < 0){
            return -1;
        }

        if(SendFinalMessage(queue, msg) < 0)
        {
            return -1;
        }

        return 0;
    }
    else
    {
        printf("Sending %s to client:%d\n", name, client);
        PacketizeData(file, queue, (long)client, priority);
    }

    return 0;
}



int DesignatePriority(const char* text,
                      char* name,
                      int* priority,
                      pid_t* client)
{

    int n = sscanf(text, "%s %d %d", name, priority, client);
    
    if (n == 3)
    {
        return 1;
    }

    return -1;
}

int PacketizeData(FILE* fp,
                  const int queue,
                  const long msg_type,
                  const int priority)
{
    Mesg snd;
    int m_size = MAXMESSAGEDATA-1;
    size_t i;

    if (priority < 0)
        m_size = MAXMESSAGEDATA-1;
    else if (priority > 1000)
        m_size = MAXMESSAGEDATA / 1000;
    else
        m_size = MAXMESSAGEDATA / priority;

    char buf[m_size];

    snd.mesg_type = msg_type;
    // Priority is organized by dividing the message by its priority number.

    while((i = fread(buf, sizeof(char), (sizeof(buf)/sizeof(char)) -1, fp)) && 
        !quit)
    {
        buf[i] = '\0';
        strcpy(snd.mesg_data, buf);

        if(SendMessage(queue, &snd) < 0){
            break;
        }

    }
    printf("Sending to %ld complete...\n", msg_type);

    SendFinalMessage(queue, &snd) < 0);
        
    fclose(fp);

    return 0;
}

/* Simple signal handler */
void sig_handler(int sig)
{

    if(sig){

    }
        
    quit = 1;
}