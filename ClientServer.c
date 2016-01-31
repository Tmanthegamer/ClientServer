#include "ClientServer.h"

extern int errno;       // error NO.
int msgQueue, rc;
int done;

int main(int argc, char **argv)
{
    struct sigaction sa;
  	struct sigaction oldint;

    sa.sa_handler = sig_handler;
  	sigemptyset (&sa.sa_mask);
  	sa.sa_flags = 0;

    sigaction (SIGINT, &sa, &oldint);
  	sigaction (SIGTSTP, &sa, NULL);

    Server();

    // Restore normal action
    sigaction (SIGINT, &oldint, NULL);

    #if 0
    long i = 5;
    key_t key;
    Mesg snd;
    Mesg rcv;
    Mesg childRCV;

    key = ftok("Info", 'a');
    msgQueue = msgget(key, 0644|IPC_CREAT|IPC_EXCL);

    if (msgQueue < 0) {
        perror(strerror(errno));
        return 1;
    }

    sprintf(snd.mesg_data, "nope");
    snd.mesg_type = i;
    snd.mesg_len = 20;
    printf("{{%s}}\n", snd.mesg_data);
    if(SendMessage(msgQueue, &snd) < 0)
    {
        printf("SendMessage");
        RemoveQueue(msgQueue);
        return 1;
    }

    if(ReadMessage(msgQueue, &rcv, i) == 0)
    //rc = msgrcv(msgQueue,&rcv,sizeof(rcv.mesg_data),i,IPC_NOWAIT);
    //if(rc >= 0)
    {
        pid_t child;
        child = fork();
        switch(child)
        {
        case -1:
            printf("Fatal error.\n");
            break;
        case 0: //child

            // I NEED A MUTEX HERE SO COPYING DOESN'T GO WRONG
            sprintf(childRCV.mesg_data, "%s", rcv.mesg_data);
            childRCV.mesg_len = rcv.mesg_len;
            childRCV.mesg_type = rcv.mesg_type;

            ProcessClient(&rcv, msgQueue, 0, 10);
            exit(1);
            break;

        default: //parent
            printf("Parent is outta here.\n");
        }

    } else {
        perror( strerror(errno) );
    }

    printf("Killed message.\n");
    RemoveQueue(msgQueue);
    #endif

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
    //printf("[Read]<%s>\n", msg->mesg_data);

    return 0;
}

int SendMessage(int queue, Mesg* msg)
{
    printf("queue:%d\n", msgQueue);
    printf("[%s][%d][%ld]", msg->mesg_data, msg->mesg_len, msg->mesg_type);
    rc = msgsnd(msgQueue,msg,sizeof(msg->mesg_data),IPC_NOWAIT);

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


int Server(void)
{
  key_t key;

  key = ftok("Info", 'a');
  msgQueue = msgget(key, 0644|IPC_CREAT|IPC_EXCL);
  printf("Queue:%d\n", msgQueue);
  //msgQueue = msgget(IPC_PRIVATE,IPC_CREAT|IPC_EXCL);

  if (msgQueue < 0) {
      perror(strerror(errno));
      return 1;
  }

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
    long type = 1;
    Mesg snd;
    sprintf(snd.mesg_data, "nope5 10");
    snd.mesg_type = type;
    snd.mesg_len = 20;
    printf("{{%s}}\n", snd.mesg_data);
    if(SendMessage(msgQueue, &snd) < 0)
    {
      printf("SendMessage\n");
      RemoveQueue(msgQueue);
      return 1;
    }

    Mesg rcv;
    int priority = 0;

    //while (1){
    if(ReadMessage(msgQueue, &rcv, CLIENT_TO_SERVER) == 0)
    {
        priority = DesignatePriority(rcv.mesg_data);
        #if 0
        DesignatePriority(rcv.mesg_data);
        pid_t child;
        child = fork();
        switch(child)
        {
        case -1:
            printf("Fatal error.\n");
            break;
        case 0: //child
            ProcessClient(&rcv, msgQueue, SERVER_TO_ALL_CLIENTS, 10);
            exit(1);
            break;

        default: //parent
            //No action to be taken for the parent
            break;
        }
        #endif
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

int ProcessClient(Mesg* msg, int queue, long PID, int priority)
{
    FILE* file;
    if((file = OpenFile(msg->mesg_data)) == NULL)
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
        PacketizeData(file, queue, PID, priority);
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

int DesignatePriority(const char* text)
{
    int priority;
    char trash[5000] = {'\0'};
    int n = sscanf(text, "%s %d", trash, &priority);
    if(n == 2){
        return priority;
    }
    return -1;
}

int PacketizeData(FILE* fp,
                  const int queue,
                  const long msg_type,
                  const int priority)
{
    int m_size = MAXMESSAGEDATA;
    char buf[m_size+1];
    int count = 0;

    // Priority is organized by dividing the message by its priority number.
    if (priority < 0)
        m_size = 1;
    else if (priority > 10)
        m_size = 10;
    else
        m_size = MAXMESSAGEDATA / priority;

    while(fgets(buf, m_size, fp) != NULL)
    {
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
    printf("Done packetizing\n");
    fclose(fp);

    return 0;
}

/* Simple signal handler */
void sig_handler(int sig)
{
    RemoveQueue(msgQueue);
    printf("Removed queue\n");
    fflush(stdout);
    /*
    ADD IN MORE CLEAN UP DUTIES HERE.
    */

    exit(1);
}
