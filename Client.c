/* 
===============================================================================
SOURCE FILE:    Client.c
                    C definition file

PROGRAM:        Client

FUNCTIONS:      int main(void)
                int Client(void);
                int PromptUserInput(char* input)
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

int main(void)
{
    struct sigaction sa;
    struct sigaction oldint;

    sa.sa_handler = sig_handler;
    sigemptyset (&sa.sa_mask);
    sa.sa_flags = 0;

    sigaction (SIGINT, &sa, &oldint);
    sigaction (SIGTSTP, &sa, NULL);

    Client();

    // Restore normal action
    sigaction (SIGINT, &oldint, NULL);

    return 0;
}

int Client(void)
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

    while (1)
    {
        if(done) {
            ready = 1;
            rc = PromptUserInput(request);
            if(rc < 0)
            {
                printf("Exiting...\n");
                thread = 0;
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
            
            printf("[%s][%zu][%ld]\n", snd.mesg_data, snd.mesg_len, snd.mesg_type);
        }
        
    }
    

    /*
    printf("%s\n", "Waiting for server...");
    while(1)
    {
        if(ReadMessage(msgQueue, &rcv, getpid()) == 0){
            if(rcv.mesg_len == 0) {
                break;
            }
            //printf("[%s][%d][%ld]\n", rcv.mesg_data, rcv.mesg_len, rcv.mesg_type);
            printf("%s", rcv.mesg_data);
        }
    }
    printf("Finished reading server response.\n");*/

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

int PromptUserInput(char* input)
{
    int len;
    int priority = 1;
    int client;

    char name[BUFF];

    printf("Please enter: [filename]\n");
    
    while((fgets(name, BUFF-1, stdin)) == NULL)
    {
        printf("Type \"help\" for instructions");
    }

    if(strstr(name, "quit") != 0)
    {
        return -1;
    } else if(strstr(name, "help"))
    {
        ClientHelp(); // Display application usage and restart this function.
        return 1;

    }

    printf("(Optional) Please enter: [priority]\n");
    fgets(input, BUFF-1, stdin);
    if(sscanf(input, "%d", &priority) != 1)
    {
        priority = 1;
    }

    printf("(Optional) Please enter: [Client Process ID]\n");
    fgets(input, BUFF-1, stdin);
    if(sscanf(input, "%d", &client) != 1)
    {
        client = getpid();
    }

    sprintf(input, "%s %d %d", name, priority, client);

    return 0;
}

void* ReadServerResponse(void* msgQueue)
{
    Mesg rcv;
    rcv.mesg_len = 0;

    while(thread)
    {
        while(ready)
        {
            if(ReadMessage((*(int*)msgQueue), &rcv, getpid()) == 0){
                if(rcv.mesg_len == 0) {
                    break;
                }
                //printf("[%s][%d][%ld]\n", rcv.mesg_data, rcv.mesg_len, rcv.mesg_type);
                printf("%s", rcv.mesg_data);
            }
        }
        printf("Finished reading server response.\n");
        done = 1;
        ready = 0;
    }

    return 0;
}

void ClientHelp(void)
{
    printf("\n===================================================\n");
    printf("               CLIENT PROGRAM USAGE                \n");
    printf("===================================================\n\n");
    printf("{This client's PID is: %d                         }\n", getpid());
    printf("Enter the: [filename]                            \n");
    printf("     (a)Wait for the Server to respond with the  \n");
    printf("        desired file contents.                 \n\n");
    printf("You have two optional options when inputing a    \n");
    printf("filename:                                        \n");
    printf("      [Priority]                                 \n");
    printf("          Changes the speed of message retrieval \n\n");
    printf("      [Client process ID]                        \n");
    printf("          Designates a process (that is currently\n");
    printf("          running this application) which will   \n");
    printf("          receive a file's contents.             \n\n");
    printf("You can close this application by entering \"quit\". \n\n")
    printf("Enter in \"help\"to to see this message again.   \n");
    printf("=================================================\n");
}

/* Simple signal handler */
void sig_handler(int sig)
{
    thread = 0;
    if(sig){

    }
    
    RemoveQueue(msgQueue);
    fflush(stdout);

    exit(1);
}