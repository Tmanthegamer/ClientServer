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


int Client(void)
{
    long type = CLIENT_TO_SERVER;
    char request[BUFF];

    Mesg snd;

    if(OpenQueue() < 0){
        return 1;
    }

    done = 1;

    while (1)
    {
        if(done) {
            done = 0;

            if(PromptUserInput(request) < 0)
            {
                printf("Exiting...\n");
                return -1;
            }

            strncpy(snd.mesg_data, request, BUFF);
            
            snd.mesg_type = type;
            snd.mesg_len = 20;
            if(SendMessage(msgQueue, &snd) < 0)
            {
              printf("SendMessage\n");
              perror( strerror(errno) );
              return -1;
            }
            
            printf("[%s][%zu][%ld]\n", snd.mesg_data, snd.mesg_len, snd.mesg_type);
            
            CreateReadThread();
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

int CreateReadThread()
{

    printf("hit\n");

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

    char name[BUFF];

    printf("\nThis client's PID is: %d\n", getpid());
    printf("You can quit by entering: \'quit\'.\n");
    printf("Please enter: [filename] [(optional)priority]\n");
    
    if ((fgets(input, BUFF-1, stdin)) == NULL)
    {
        printf("error in user input.\n");
        return -1;
    }

    len = strlen(input);
    input[len-1] = '\0'; //Remove the newline character

    if(sscanf(input, "%s %d", name, &priority) == 1){
        priority = 1;
    }

    sprintf(input, "%s %d %d", name, priority, getpid());
    
    if(strstr(name, "quit") != 0)
    {
        return -1;
    }


    return 0;
}

void* ReadServerResponse(void* msgQueue)
{
    Mesg rcv;
    rcv.mesg_len = 0;

    while(1)
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

    return 0;
}
