#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

struct mymsgbuf
{
  long mtype;
  struct {
    pid_t pid;
    char text[81];
  } info;
};

void *mythread(void *arg)
{
   int msqid;
   char pathname[]="09-4Server.c";
   key_t  key;
   struct mymsgbuf* mybuf = (struct mymsgbuf *)arg;
   int len;

   if((key = ftok(pathname,0)) < 0){
      printf("Can\'t generate key\n");
      exit(-1); 
   }
    
   if ((msqid = msgget(key, 0666 | IPC_CREAT)) < 0){
      printf("Can\'t get msqid\n");
      exit(-1);
   }

   mybuf->mtype = mybuf->info.pid;
   strcpy(mybuf->info.text, "Was served");
   len = strlen(mybuf->info.text)+5;

   printf("%d was get\n", mybuf->info.pid);
       
   if (msgsnd(msqid, (struct msgbuf *) mybuf, len, 0) < 0){
      printf("Can\'t send message to queue\n");
      msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
      exit(-1);
    }

   return NULL;
}

int main(void)
{
    
    int msqid;
    char pathname[]="09-4Server.c";
    key_t  key;
    int len, maxlen;
    struct mymsgbuf mybuf;
    pthread_t thid;
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
    
    while(1){
       
       maxlen = sizeof(struct mymsgbuf);
       
       if (( len = msgrcv(msqid, (struct msgbuf *) &mybuf, maxlen, 1, 0)) < 0){
         printf("Can\'t receive message from queue\n");
         exit(-1);
       }
       
       int result = pthread_create( &thid, (pthread_attr_t *)NULL, mythread, &mybuf);
   
       if(result != 0){
        printf ("Error on thread create, return value = %d\n", result);
        exit(-1);
       } 

    }    

    return 0;    
}
