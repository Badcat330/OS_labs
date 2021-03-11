#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void)
{
    
    int msqid;
    char pathname[]="09-4Server.c";
    key_t  key;
    int len, maxlen;
    pid_t pid = getpid();

    struct mymsgbuf
    {
       long mtype;
       struct {
        pid_t pid;
        char text[81];
       } info;
    } mybuf;

    /* Create or attach message queue  */
    
    if((key = ftok(pathname,0)) < 0){
      printf("Can\'t generate key\n");
      exit(-1); 
    }
    
    if ((msqid = msgget(key, 0666 | IPC_CREAT)) < 0){
       printf("Can\'t get msqid\n");
       exit(-1);
    }


    /* Send information */
       
     mybuf.mtype = 1;
     mybuf.info.pid = pid;
     strcpy(mybuf.info.text, "This is text message from client");
     len = strlen(mybuf.info.text) + 5;
     
     if (msgsnd(msqid, (struct msgbuf *) &mybuf, len, 0) < 0){
       printf("Can\'t send message to queue\n");
       msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
       exit(-1);
     }   

    /* Read information */
       
    maxlen = sizeof(struct mymsgbuf);
    
    if (( len = msgrcv(msqid, (struct msgbuf *) &mybuf, maxlen, pid, 0)) < 0){
      msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
      printf("Can\'t receive message from queue\n");
      exit(-1);
    }
    
    printf("message type = %ld, info = %s\n", mybuf.mtype, mybuf.info.text);

    return 0;    
}
