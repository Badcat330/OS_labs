#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    
    int msqid;
    char pathname[]="09-3a.c";
    key_t  key;
    int i,len, maxlen;

    struct mymsgbuf
    {
       long mtype;
       char mtext[81];
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
    
    for (i = 1; i <= 5; i++){
       
       mybuf.mtype = 1;
       strcpy(mybuf.mtext, "This is text message from 09-3a");
       len = strlen(mybuf.mtext)+1;
       
       if (msgsnd(msqid, (struct msgbuf *) &mybuf, len, 0) < 0){
         printf("Can\'t send message to queue\n");
         msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
         exit(-1);
       }
    }    

    /* Read information */
    for (i = 1; i <= 5; i++){
       
       maxlen = 81;
       
       if (( len = msgrcv(msqid, (struct msgbuf *) &mybuf, maxlen, 2, 0)) < 0){
         printf("Can\'t receive message from queue\n");
         exit(-1);
       }
       
       printf("message type = %ld, info = %s\n", mybuf.mtype, mybuf.mtext);
    }

    return 0;    
}
