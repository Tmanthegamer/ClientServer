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

    OpenQueue();
    /*if(OpenQueue() < 0)
    {
        perror(strerror(errno));
        return -1;
    }*/

    if(PromptUserInput(request) < 0)
    {
        printf("Exiting...\n");
        return -1;
    }
    sprintf(snd.mesg_data, request);
    snd.mesg_type = type;
    snd.mesg_len = 20;
    if(SendMessage(msgQueue, &snd) < 0)
    {
      printf("SendMessage\n");
      perror( strerror(errno) );
      return -1;
    }
    printf("[%s][%d][%ld]\n", snd.mesg_data, snd.mesg_len, snd.mesg_type);
    Mesg rcv;


    printf("%s\n", "Waiting for server...");
    while(1)
    {
        if(ReadMessage(msgQueue, &rcv, SERVER_TO_ALL_CLIENTS) == 0){
            if(strstr(rcv.mesg_data, "done") != 0) {
                break;
            }
            //printf("[%s][%d][%ld]\n", rcv.mesg_data, rcv.mesg_len, rcv.mesg_type);
            printf("%s", rcv.mesg_data);
        }
    }

    return 0;
}

void* OutputFunction(void* message)
{
    printf("%s", (char*)message);
    pthread_exit((void*)0);
}

int DisplayMessage(const char* message)
{
    pthread_attr_t detach_attr;
    pthread_t thread;

    pthread_attr_init(&detach_attr);
    pthread_attr_setdetachstate(&detach_attr, PTHREAD_CREATE_DETACHED);
    rc = pthread_create(&thread, &detach_attr, OutputFunction, (void*)message);

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



    return 0;
}

int ReadServerResponse(void);

int TerminateClient(void);
