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
#include "ClientServer.h"

/*
    TODO: 
    -Extract ProcessInput from the LinuxTerminal for user input
    -Use that Process Input to allow the user to exit the application
    -Fill in the Cleanup function.
*/

extern int errno;       // error NO.

int main(int argc, char **argv)
{
    struct sigaction sa;
  	struct sigaction oldint;

    if(argc != 2){
        printf("Two arguments.\n");
        return 1;
    }

    if(argv[1][0] == 's')
    {
        sa.sa_handler = sig_handler;
      	sigemptyset (&sa.sa_mask);
      	sa.sa_flags = 0;

        sigaction (SIGINT, &sa, &oldint);
      	sigaction (SIGTSTP, &sa, NULL);

        Server();

        // Restore normal action
        sigaction (SIGINT, &oldint, NULL);
    }
    else if (argv[1][0] == 'c')
    {
        Client();
    }

    return 0;
}

int ReadMessage(int queue, Mesg* msg, long msg_type)
{
    rc = msgrcv(queue,msg,sizeof(msg->mesg_data),msg_type,IPC_NOWAIT);
    if(rc < 0)
    {
        //perror( strerror(errno) );
        //printf("msgrcv failed, rc=%d\n", rc);
        return -1;
    }
    //printf("[Read]<%d>\n", rc);

    return 0;
}

int ReadAllMessages(int queue, Mesg* msg, long msg_type)
{
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
    /*
    Grab assigned PID to indicate message type (destination of message)
    Grab the message queue
    Grab the message
    Grab the “more” flag to indicate more messages
    Send message to the message queue using the message type as its address.
    IF “more” flag has been specified
        Return to PACKETIZE DATA (restart its file read loop)
        Goto PACKETIZE DATA
    */
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


int Server(void)
{

    if(OpenQueue() < 0)
        return 1;

    //Insert ProcessInput functional inside here.
    SearchForClients();

    RemoveQueue(msgQueue);
    /*
    Create ClientServer msg queue
    Assign Message-Type 1 to be the messages designated Client to Server
    Goto SEARCH FOR CLIENTS
    */
    return 0;
}

int SearchForClients(void)
{
    #if 0
    Mesg snd;

    strcpy(snd.mesg_data, "nope5 10");
    snd.mesg_type = CLIENT_TO_SERVER;

    if(SendMessage(msgQueue, &snd) < 0)
    {
      printf("SendMessage\n");
      //RemoveQueue(msgQueue);
      return 1;
    }
    printf("[SEND: len=%zu]\n", snd.mesg_len);
    #endif

    Mesg rcv;

    while (1){
        if(ReadMessage(msgQueue, &rcv, CLIENT_TO_SERVER) == 0)
        {

            //mesg_len doesn't work
            printf("normal:[msg:%s][len:%zu][type:%ld]\n", rcv.mesg_data, rcv.mesg_len, rcv.mesg_type);
            pid_t child;
            child = fork();
            switch(child)
            {
            case -1:
                printf("Fatal error.\n");
                break;
            case 0: //child

                ProcessClient(&rcv, msgQueue);
                exit(1);
                break;

            default: //parent
                //No action to be taken for the parent
                break;
            }

        }

    }


    printf("Killed SearchForClients.\n");
    /*
    Check for any incoming messages from clients.
    Failure no new messages: RESTART SEARCH FOR CLIENTS
    Pass message to CREATE NEW CLIENT PROCESS.
    Goto CREATE NEW CLIENT PROCESS
    */
    return 0;
}

int CreateNewClient(void)
{

    /*
    Check message data for client’s PID
    Check message for client‘s priority
        IF no priority specified, set priority to maximum
    Fork a child process
    IF Process is the child
    Pass client information to this process.
    Assign the task START (process)
    Goto START (process)
    ELSE
        This is the Parent Process
    Goto SEARCH FOR CLIENTS
    */
    return 0;
}

int ProcessClient(Mesg* msg, int queue)
{
    FILE* file;
    char name[BUFF];
    pid_t client;
    int priority;

    printf("ProcessClient\n");
    if(DesignatePriority(msg->mesg_data, name, &priority, &client) < 0)
    {
        printf("error");
    }

    printf("Opening file.\n");
    if((file = OpenFile(name)) == NULL)
    {
        msg->mesg_type = client;
        sprintf(msg->mesg_data, "Cannot open file: %s\n\0", name);
       if(SendMessage(queue, msg) < 0){
            
        }

        if(SendFinalMessage(queue, msg) < 0)
        {
            printf("Can't print final message.\n");
        }

        /*
        Failure: File cannot be opened for reading
            Send “Error: Cannot open file” message to SEND MESSAGE
            Goto SEND MESSAGE
        */
        return -1;
    }
    else
    {
        PacketizeData(file, queue, (long)client, priority);
    }

    /*
    Grab received message
    Get Client’s Priority and PID
    Goto OPEN REQUESTED FILE
    */

    return 0;
}

FILE* OpenFile(const char* fileName)
{
    FILE *fp;

    fp = fopen(fileName, "r"); // read file
    //fclose(fp);

    return fp;
}

int DesignatePriority(const char* text,
                      char* name,
                      int* priority,
                      pid_t* client)
{

    int n = sscanf(text, "%s %d %d", name, priority, client);

    if(n == 2){
        *client = SERVER_TO_ALL_CLIENTS;
        return 0;
    }
    else if (n == 3)
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
    int m_size = MAXMESSAGEDATA;
    int count = 0;


    if (priority < 0)
        m_size = MAXMESSAGEDATA;
    else if (priority > 1000)
        m_size = MAXMESSAGEDATA / 1000;
    else
        m_size = MAXMESSAGEDATA / priority;

    char buf[m_size];

    snd.mesg_type = msg_type;
    // Priority is organized by dividing the message by its priority number.
    printf("PacketizeData.\n");
    printf("[m_size:%d]", m_size);

    //don't use fgets, copy each char individually until the limit is filled.
    while(fgets(buf, m_size, fp) != NULL)
    {
        strcpy(snd.mesg_data, buf);

        if(SendMessage(queue, &snd) < 0){
            printf("Error in PacketizeData, SendMessage fail\n");
        }

        //printf("<Portion: %d>\n\n%s", count++, buf);
        /*
        Send the portion of the message to SEND MESSAGES
        Send the PID to SEND MESSAGES
        Send the Message Queue to SEND MESSAGES
        Send the “more” flag to SEND MESSAGES
        Goto SEND MESSAGES
        Restart loop
        */
    }
    printf("Done, sending final message\n");
    if(SendFinalMessage(queue, &snd) < 0){
        printf("Error in ending msg, SendMessage fail\n");
    }
    fclose(fp);

    return 0;
}

/* Simple signal handler */
void sig_handler(int sig)
{
    if(sig){

    }
    RemoveQueue(msgQueue);
    fflush(stdout);
    /*
    ADD IN MORE CLEAN UP DUTIES HERE.
    */

    exit(1);
}


void Cleanup(void)
{

}