#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/wait.h>
#include <sys/errno.h>
     
extern int errno;       // error NO.
#define MSGPERM 0644    // msg queue permission
#define MSGTXTLEN 1024   // msg text length

int msgqid, rc;
int done;

struct msg_buf {
    long mtype;
    long priority;
    char mtext[MSGTXTLEN];
};

int ReadMessage(int, struct msg_buf, size_t, long, int);

int SendMessage(int, struct msg_buf*, int);

int RemoveQueue(int);


int main(int argc,char **argv)
{
    int clientToServer;
    int serverToClient;
    struct msg_buf* msg;
    key_t key1, key2;

    key1 = ftok("Info", 'a');
    key2 = ftok("Info", 'b');

    msg = malloc(sizeof (struct msg_buf));
    if(msg == 0)
        return 0;
    
    printf("Program Start\n");
    clientToServer = msgget(key1, MSGPERM|IPC_CREAT|IPC_EXCL);
    // create a message queue. If here you get a invalid msgid and use it in msgsnd() or msgrcg(), an Invalid Argument error will be returned.  msgqid = msgget((key_t)0600, MSGPERM|IPC_CREAT|IPC_EXCL);
    if (clientToServer < 0) {
        perror(strerror(errno));
        printf("failed to create message queue with clientToServer = %d\n", clientToServer);
        return 1;
    }
    printf("message queue %d created\n",clientToServer);

    serverToClient = msgget(key2, MSGPERM|IPC_CREAT|IPC_EXCL);
    // create a message queue. If here you get a invalid msgid and use it in msgsnd() or msgrcg(), an Invalid Argument error will be returned.  msgqid = msgget((key_t)0600, MSGPERM|IPC_CREAT|IPC_EXCL);
    if (serverToClient < 0) {
        perror(strerror(errno));
        printf("failed to create message queue with serverToClient = %d\n", serverToClient);
        return 1;
    }
    printf("message queue %d created\n",msgqid);
  
    printf("Send clientToServer message\n");
    if(SendMessage(clientToServer, msg, 0) != 0){
        printf("SendMessage failed\n");
        RemoveQueue(clientToServer);
        RemoveQueue(serverToClient);
        return 1;
    }
    printf("Sent success!\n");

    // read the message from queue
    rc = msgrcv(clientToServer, &msg, sizeof(msg->mtext), 0, 0); 
    if (rc < 0) {
        perror( strerror(errno) );
        printf("msgrcv failed, rc=%d\n", rc);
        RemoveQueue(clientToServer);
        RemoveQueue(serverToClient);
        return 1;
    } 
    printf("received msg: %s\n", msg->mtext);
    printf("priority is : %ld\n", msg->priority);

    RemoveQueue(clientToServer);
    RemoveQueue(serverToClient);

    return 0;
}

/*int ReadMessage(int id, struct msg_buf msg, size_t msg_size, long msg_type, int flag)
{
    return 0;
}*/

int SendMessage(int id, struct msg_buf* msg, int flag)
{
    static int x = 0;
    // message to send
    msg->mtype = 0; // client who sent the message
    msg->priority = x++; // set the priority of the message
    sprintf (msg->mtext, "%s %d\n", "msg:", x); /* setting the right time format by means of ctime() */

    // send the message to queue
    rc = msgsnd(id, &msg, sizeof(msg->mtext), flag); // the last param can be: 0, IPC_NOWAIT, MSG_NOERROR, or IPC_NOWAIT|MSG_NOERROR.
    if (rc < 0) {
        perror( strerror(errno) );
        printf("msgsnd failed, rc = %d\n", rc);
        return 1;
    }
    
    return 0;
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