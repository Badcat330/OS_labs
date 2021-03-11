#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

struct mymsgbuf
{
  long mtype;
  struct {
    pid_t pid;
    int n;
  } info;
};

int msqid;
char pathname[]="09-5.c";
key_t  key;
int len, maxlen;
struct mymsgbuf mybuf;
int i = 0;
int count = 0;
struct mymsgbuf queue[4096];

void deleteN(int index)
{
  for (int j = index; j < i - 1; ++j)
  {
    queue[j] = queue[j + 1];
  }

  i--;
}

void find()
{
  for (int j = 0; j < i; ++j)
  {
    if(count >= -queue[j].info.n)
    {
        count += queue[j].info.n;

        struct mymsgbuf mybuf;

        mybuf.mtype = queue[j].info.pid;
        len = 0;
            
        if (msgsnd(msqid, (struct msgbuf *) &mybuf, len, 0) < 0)
        {
           printf("Can\'t send message to queue\n");
           msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
           exit(-1);
        }

        deleteN(j);
        find();
        break;
    }
  }
}

int main(void)
{
    

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
       
       if(mybuf.info.n > 0)
       {
          count += mybuf.info.n;

          mybuf.mtype = mybuf.info.pid;
          len = 0;
              
          if (msgsnd(msqid, (struct msgbuf *) &mybuf, len, 0) < 0){
             printf("Can\'t send message to queue\n");
             msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
             exit(-1);
          }

          find();
       }
       else
       {
          if(count >= -mybuf.info.n)
          {
            count += mybuf.info.n;

            mybuf.mtype = mybuf.info.pid;
            len = 0;
                
            if (msgsnd(msqid, (struct msgbuf *) &mybuf, len, 0) < 0){
               printf("Can\'t send message to queue\n");
               msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
               exit(-1);
            }
          }
          else
          {
            queue[i] = mybuf;
            i++;
          }
       }
    }    

    return 0;    
}
