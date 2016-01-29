#include "ClientServer.h"
     
extern int errno;       // error NO.
int msgqid, rc;
int done;

int main(int argc, char **argv)
{
    FILE* file;

    int msgQueue;
    int i = 0;
    key_t key1;
    Mesg* rcv;

    key1 = ftok("Info", 'a');
    msgQueue = msgget(key1, 0666|IPC_CREAT|IPC_EXCL);
    if (msgQueue < 0) {
        perror(strerror(errno));
        return 1;
    }

    if(SendMessage(msgQueue, i, argv[1]) < 0) {
        printf("SendMessage");
        RemoveQueue(msgQueue);
        return 1;
    }

    rcv = malloc(sizeof(Mesg));
    if(rcv == NULL)
        return 1;

    if(ReadMessage(msgQueue, rcv, i) < 0)
    {
        printf("ReadMessage");
        RemoveQueue(msgQueue);
        free(rcv);
        return 1;
    }

    if((file = OpenFile(rcv->mesg_data)) == NULL)
    {
        printf("Cannot open file.\n");
        /*
        Failure: File cannot be opened for reading
            Send “Error: Cannot open file” message to SEND MESSAGE
            Goto SEND MESSAGE
        */
    }
    else 
    {
        PacketizeData(file, 0, 0, 5);
    }
    
    free(rcv);
    RemoveQueue(msgQueue);
    return 0;
}

int ReadMessage(int queue, Mesg* msg, long msg_type)
{
    if( (rc = msgrcv(queue, &msg, sizeof(msg), msg_type, IPC_NOWAIT)) < 0) 
    {
        perror( strerror(errno) );
        printf("msgrcv failed, rc=%d\n", rc);
        return -1;
    }
    //printf("[Read]<%s>\n", msg->mesg_data);

    return 0;
}

int SendMessage(int queue, long type, char* data)
{
    Mesg* msg = malloc(sizeof(Mesg));
    if(msg == NULL)
        return -1;
    // message to send
    msg->mesg_type = type; // client who sent the message
    sprintf(msg->mesg_data, "%s", data);
    msg->mesg_len = sizeof(msg->mesg_data);
    
    rc = msgsnd(queue, &msg, sizeof(msg), IPC_NOWAIT);
    free(msg);

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
    /*
    Create ClientServer msg queue
    Assign Message-Type 1 to be the messages designated Client to Server
    Goto SEARCH FOR CLIENTS
    */
    return 0;
}

int SearchForClients(void)
{
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

int ProcessClient(Mesg* msg, int priority, long PID, int queue)
{
    FILE* file;
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

int PacketizeData(FILE* fp, 
                  const int queue, 
                  const long msg_type, 
                  const int priority)
{
    const int m_size = 4096 / priority;
    char buf[m_size+1];
    int count = 0;

    while(fgets(buf, m_size, fp) != NULL)
    {
        printf("<Portion: %d>\n\n%s", count++, buf);
        /*
        Send the portion of the message to SEND MESSAGES
        Send the PID to SEND MESSAGES
        Send the Message Queue to SEND MESSAGES
        Send the “more” flag to SEND MESSAGES
        Goto SEND MESSAGES
        Restart loop
        */
    }

    fclose(fp);

    return 0;
}