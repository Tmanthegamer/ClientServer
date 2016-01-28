#include "ClientServer.h"
     
extern int errno;       // error NO.
int msgqid, rc;
int done;

int main(int argc,char **argv)
{
    int msgQueue;
    int i = 0;
    Mesg* msg[5];
    key_t key1;

    key1 = ftok("Info", 'a');
    
    printf("Program Start\n");
    msgQueue = msgget(key1, 0666|IPC_CREAT|IPC_EXCL);
    // create a message queue. If here you get a invalid msgid and use it in msgsnd() or msgrcg(), an Invalid Argument error will be returned.  msgqid = msgget((key_t)0600, MSGPERM|IPC_CREAT|IPC_EXCL);
    if (msgQueue < 0) {
        perror(strerror(errno));
        printf("failed to create message queue with msgQueue = %d\n", msgQueue);
        return 1;
    }
    printf("message queue %d created\n",msgQueue);
  
    printf("Send msgQueue message\n");
    for(i = 0; i < 5; i++){
        msg[i] = malloc(sizeof (Mesg));
        if(msg[i] == 0)
            return 1;
        if(SendMessage(msgQueue, msg[i], i, 0)){
            printf("SendMessage failed\n");
            RemoveQueue(msgQueue);
        return 1;
        }
    }
    /*if(SendMessage(msgQueue, msg, 0) != 0){
        printf("SendMessage failed\n");
        RemoveQueue(msgQueue);
        return 1;
    }*/
    printf("Sent success!\n");

    // read the message from queue
    Mesg* test = malloc(sizeof(Mesg));
    while(ReadMessage(msgQueue, test, sizeof(Mesg), 0, IPC_NOWAIT) == 0) {
        printf("Success!\n");
    }
    RemoveQueue(msgQueue);

    free(test);
    for(i = 4; i > -1; i--) {
        free(msg[i]);
    }

    return 0;
}

int ReadMessage(int id, Mesg* msg, size_t msg_size, long msg_type, int flag)
{
    if( (rc = msgrcv(id, &msg, msg_size, 0, IPC_NOWAIT)) < 0) 
    {
        perror( strerror(errno) );
        printf("msgrcv failed, rc=%d\n", rc);
        return 1;
    }
    printf("%s\n", msg->mesg_data);

    return 0;
}

int SendMessage(int id, Mesg* msg, int priority, int flag)
{
    // message to send
    msg->mesg_type = 0; // client who sent the message
    sprintf (msg->mesg_data, "%s %d", "Texterino==>", priority); /* setting the right time format by means of ctime() */

    // send the message to queue
    printf("%s\n",msg->mesg_data);
    rc = msgsnd(id, &msg, sizeof(msg), IPC_NOWAIT); // the last param can be: 0, IPC_NOWAIT, MSG_NOERROR, or IPC_NOWAIT|MSG_NOERROR.
    if (rc < 0) {
        perror( strerror(errno) );
        printf("msgsnd failed, rc = %d\n", rc);
        return 1;
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

int RemoveQueue(int id)
{
    int rc = msgctl(id, IPC_RMID, NULL);
    if (rc < 0) {
        perror( strerror(errno) );
        printf("msgctl (return queue) failed, rc=%d\n", rc);
        return 1;
    }
    printf("message queue %d is gone\n",id);

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

int ProcessClient(void)
{
    /*
    Begin DESIGNATE PRIORITY
    */
    return 0;
}

int DesignatePriority(void)
{
    /*

    Get Client’s Priority and PID
    Designate new length of message to be MAXSIZE / PRIORITY
    Goto OPEN REQUESTED FILE
    */
    return 0;
}

int OpenFile(void)
{
    /*
    Get message data from client
    Read desired Filename from message data
    Open requested file
        Failure: File cannot be opened for reading
            Send “Error: Cannot open file” message to SEND MESSAGE
            Goto SEND MESSAGE

        Success: File has been read
            Send this file to PACKETIZE DATA
            Goto PACKETIZE DATA
    */
    return 0;
}

int PacketizeData(void)
{
    /*
    Grab file contents
    Grab the message queue
    Grab the PID
    Grab client priority
    Read file until designated length of message
    WHILE the file can be read
        Check if file can be read
        IF file cannot be read
            Get out of while loop
    Send the portion of the message to SEND MESSAGES
        Send the PID to SEND MESSAGES
        Send the Message Queue to SEND MESSAGES
        Send the “more” flag to SEND MESSAGES
        Goto SEND MESSAGES
        Restart loop
    File has finished reading
    Goto END (process)  
    */
    return 0;
}