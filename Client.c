/*
===============================================================================
SOURCE FILE:    LinuxTerminal.c
                    words go here

PROGRAM:        Linux Terminal

FUNCTIONS:      void function(void)
                int functionTwo(int pants)
                char* functionThree(char word)

DATE:           January 9, 2016

REVISIONS:

DESIGNGER:      Tyler Trepanier-Bracken

PROGRAMMER:     Tyler Trepanier-Bracken

NOTES:
Standard Notes go here.
===============================================================================
*/


#include "Client.h"
/*
CURRENT ISSUES:

MESSAGES SHOULD BE IN THE FORMAT OF <FILENAME> <PRIORITY> <CLIENT>
HOWEVER WHENEVER THERE'S ONLY <FILENAME> INPUTTED BY THE user
    THE RESULTING MESSAGE WILL BE <FILENAME> <CLIENT>
*/
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
    /*
    ADD IN MORE CLEAN UP DUTIES HERE.
    */

    exit(1);
}