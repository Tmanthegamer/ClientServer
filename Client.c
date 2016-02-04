/* 
===============================================================================
SOURCE FILE:    Client.c
                    C definition file

PROGRAM:        Client

FUNCTIONS:      int main(void)
                int Client(void);
                int ReadArguments(char* request, int argc, char** argv)
                int CreateReadThread(void);
                void* ReadServerResponse(void *queue);
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

The sig_handler is defined in this file to account for the detached pthread.
===============================================================================
*/

#include "Client.h"

int main(int argc, char** argv)
{

    sa.sa_handler = sig_handler;
    sigemptyset (&sa.sa_mask);
    sa.sa_flags = 0;

    sigaction (SIGINT, &sa, &oldint);
    sigaction (SIGTSTP, &sa, NULL);

    Client(argc, argv);

    // Restore normal action
    sigaction (SIGINT, &oldint, NULL);

    return 0;
}

int Client(int argc, char** argv)
{
    long type = CLIENT_TO_SERVER;
    char request[BUFF];

    Mesg snd;

    if(OpenQueue() < 0){
        return 1;
    }

    done = 1;

    if(CreateReadThread() < 0)
        return -1;

    while (running)
    {
        if(done) {

            rc = ReadArguments(request, argc, argv);
            if(rc < 0)
            {

                return 0;
            } 
            else if(rc == 1)
            {

                continue;
            }

            done = 0;

            strncpy(snd.mesg_data, request, BUFF);

            snd.mesg_type = type;
            if(SendMessage(msgQueue, &snd) < 0)
            {

              perror( strerror(errno) );
              return -1;
            }

        }
        
    }

    return 0;
}

int CreateReadThread(void)
{

    pthread_attr_t detach_attr;
    pthread_t thread;

    pthread_attr_init(&detach_attr);
    pthread_attr_setdetachstate(&detach_attr, PTHREAD_CREATE_DETACHED);
    rc = pthread_create(&thread, &detach_attr, ReadServerResponse, (void*)&msgQueue);

    if(rc != 0)
    {
        return -1;
    }
    return 0;
}

int ReadArguments(char* request, int argc, char** argv)
{
    int priority;
    int client = getpid();

    char name[BUFF];

    //Command line usage: ./Client [filename] [priority]
    if(argc >= 2)
    {
        if(sscanf(argv[1], "%s", name) != 1)
        {
            ClientHelp();
            return -1;
        }

        if(argc == 2 || sscanf(argv[2], "%d", &priority) != 1)
        {
            priority = 1;
        }

    } 
    else
    {
        ClientHelp();
        return -1;
    }

    //The priority can only be in between 1 and 1000.
    if(priority < 1)
    {
        priority = 1;
    } 
    else if(priority > 1000)
    {
        priority = 1000;
    }

    sprintf(request, "%s %d %d", name, priority, client);

    return 0;
}

void* ReadServerResponse(void* msgQueue)
{
    Mesg rcv;
    rcv.mesg_len = 0;

    while(1)
    {     

        fflush(stdout);

        if(ReadMessage((*(int*)msgQueue), &rcv, getpid()) == 0){
            if(rcv.mesg_len == 0) {
                break;
            }
            
            printf("%s", rcv.mesg_data);
        }

    }

    running = 0;

    return 0;
}

void ClientHelp(void)
{
    printf("Usage: [Filename] [Priority].\n");
    printf("Please note that priority is optional.\n");
}

/* Simple signal handler */
void sig_handler(int sig)
{
    if(sig){

    }

    sigaction (SIGINT, &oldint, NULL);
    fflush(stdout);

    exit(1);
}