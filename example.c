 #include <sys/msg.h>
 #include <unistd.h>
 #include <sys/types.h>
 #include <stdio.h>
 #include <string.h>

 typedef struct mymsg {
  long mtype;
  char mtext[24];
 }mymsg;

 int main()
 {
  int msqid;
  mymsg msg,buff;
  key_t key = ftok("Info", 'a');
  msqid=msgget(key, 0644|IPC_CREAT|IPC_EXCL);

  if(msqid==-1){
  perror("FAiled to create message queue\n");
  }
  else{
  printf("Message queue id:%u\n",msqid);
  }
  msg.mtype=5; // was there failed to copy
  strcpy(msg.mtext,"This is a message");
  if(msgsnd(msqid,&msg,sizeof(msg.mtext),0)==-1){
   perror("msgsnd failed:");
  }
  else{
   printf("Message sent successfully\n");
  }
 //ssize_t msgrcv(int msqid, void *msgp, size_t msgsz, long msgtyp,int msgflg);

  msgrcv(msqid,&buff,sizeof(msg.mtext),5,0);
  printf("The message received is: %s\n",buff.mtext);
  msgctl(msqid, IPC_RMID, NULL);
 }