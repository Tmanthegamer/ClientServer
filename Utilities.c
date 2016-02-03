#include "Utilities.h"

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

FILE* OpenFile(const char* fileName)
{
    FILE *fp;

    fp = fopen(fileName, "r"); // read file
    //fclose(fp);

    return fp;
}