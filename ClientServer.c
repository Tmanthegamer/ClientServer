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


extern int errno;       // error NO.
int done;

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

int SendMessage(int queue, Mesg* msg)
{
    msg->mesg_len = sizeof(msg->mesg_data);
    rc = msgsnd(queue,msg,sizeof(msg->mesg_data),IPC_NOWAIT);

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

int RemoveQueue(int queue)
{
    int rc;
    if (( rc= msgctl(queue, IPC_RMID, NULL) ) < 0)
    {
        perror( strerror(errno) );
        printf("msgctl (return queue) failed, rc=%d\n", rc);
        return 1;
    }
    return 0;
}

int ParseCommandLine(void)
{
    /*
    Read the command line arguments
    IF command line specifies server
        Goto SERVER
    ELSE
        Goto Client
    */
    return 0;
}

int OpenNewQueue(void)
{
    key_t key;

    key = ftok("Info", 'a');
    msgQueue = msgget(key, 0644|IPC_CREAT|IPC_EXCL);
    printf("Queue:%d\n", msgQueue);
    //msgQueue = msgget(IPC_PRIVATE,IPC_CREAT|IPC_EXCL);

    if (msgQueue < 0) {
        return -1;
    }

    return 0;
}

int OpenQueue(void)
{
    key_t key;

    key = ftok("Info", 'a');
    msgQueue = msgget(key, 0644|IPC_EXCL);
    printf("Queue:%d\n", msgQueue);
    //msgQueue = msgget(IPC_PRIVATE,IPC_CREAT|IPC_EXCL);

    if (msgQueue < 0) {
        return -1;
    }

    return 0;
}


int Server(void)
{

    if(OpenNewQueue() < 0)
    {
        OpenQueue();
    }

    SearchForClients();

    //RemoveQueue(msgQueue);
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
    snd.mesg_len = 20;

    if(SendMessage(msgQueue, &snd) < 0)
    {
      printf("SendMessage\n");
      //RemoveQueue(msgQueue);
      return 1;
    }
    printf("[SEND: len=%d]\n", snd.mesg_len);
    #endif
    Mesg rcv;

    //while (1){
    if(ReadMessage(msgQueue, &rcv, CLIENT_TO_SERVER) == 0)
    {

        //mesg_len doesn't work
        printf("normal:[msg:%s][len:%d][type:%ld]\n", rcv.mesg_data, rcv.mesg_len, rcv.mesg_type);
        pid_t child;
        child = fork();
        switch(child)
        {
        case -1:
            printf("Fatal error.\n");
            break;
        case 0: //child

            //ProcessClient(&rcv, msgQueue);
            exit(1);
            break;

        default: //parent
            //No action to be taken for the parent
            break;
        }

    }

    //}


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

    if(DesignatePriority(msg->mesg_data, name, &priority, &client) < 0)
    {
        printf("error");
    }

    if((file = OpenFile(name)) == NULL)
    {
        printf("Cannot open file.\n");
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
        m_size = 1;
    else if (priority > 10)
        m_size = 10;
    else
        m_size = MAXMESSAGEDATA / priority;

    char buf[m_size];

    snd.mesg_type = msg_type;
    // Priority is organized by dividing the message by its priority number.


    //don't use fgets, copy each char individually until the limit is filled.
    while(fgets(buf, m_size, fp) != NULL)
    {
        strcpy(snd.mesg_data, buf);
        snd.mesg_len = strlen(buf);
        count++;
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

    printf("Done packetizing <%d>\n", count);
    sprintf(snd.mesg_data, "");
    snd.mesg_len = 0;
    if(SendMessage(queue, &snd) < 0){
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
    //RemoveQueue(msgQueue);
    printf("Removed queue\n");
    fflush(stdout);
    /*
    ADD IN MORE CLEAN UP DUTIES HERE.
    */

    exit(1);
}
