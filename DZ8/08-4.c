#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/sem.h>

int main()
{
   int     fd[2], result;
   int N = 10;

   if(pipe(fd) < 0){
     printf("Can\'t open pipe\n");
     exit(-1);
   }

   result = fork();

   if(result < 0) {
      printf("Can\'t fork child\n");
      exit(-1);
   } else if (result > 0) {

     /* Parent process */
      int   semid;
      char pathname[]="08-4.c";
      key_t key;
      struct sembuf mybuf;
      char  resstring[22];

      if((key = ftok(pathname,0)) < 0){
        printf("Can\'t generate key\n");
        exit(-1);
      }
    
      if((semid = semget(key, 1, 0666 | IPC_CREAT)) < 0){
        printf("Can\'t create semaphore set\n");
        exit(-1);
      }

      mybuf.sem_num = 0;
      mybuf.sem_flg = 0;

      for(int i = 0; i < N; i++){
        mybuf.sem_op  = 0;

        if(semop(semid, &mybuf, 1) < 0){
          printf("Can\'t wait for condition\n");
          exit(-1);
        }

        if(write(fd[1], "Get a ball from parent\n", 23) != 23){
          printf("Can\'t write all string to pipe\n");
          exit(-1);
        }

        mybuf.sem_op = 2;
        
        if(semop(semid, &mybuf, 1) < 0){
          printf("Can\'t wait for condition\n");
          exit(-1);
        }

        mybuf.sem_op = 0;

        if(semop(semid, &mybuf, 1) < 0){
          printf("Can\'t wait for condition\n");
          exit(-1);
        }

        if(read(fd[0], resstring, 22) < 0){
          printf("Can\'t read string from pipe\n");
          exit(-1);
        }

        printf("%s", resstring);
      }

      if(semctl(semid, 0, IPC_RMID, 0) < 0){
          printf("Can\'t delete semafor\n");
          exit(-1);
      }

      close(fd[0]);
      close(fd[1]);
      printf("Parent exit\n");

   } else {

      /* Child process */
      int   semid;
      char pathname[]="08-4.c";
      key_t key;
      struct sembuf mybuf;
      char  resstring[23];

      if((key = ftok(pathname,0)) < 0){
        printf("Can\'t generate key\n");
        exit(-1);
      }
    
      if((semid = semget(key, 1, 0666 | IPC_CREAT)) < 0){
        printf("Can\'t create semaphore set\n");
        exit(-1);
      }

      mybuf.sem_num = 0;
      mybuf.sem_flg = 0;
      
      for (int i = 0; i < N; ++i)
      {

        mybuf.sem_op = -1;
        
        if(semop(semid, &mybuf, 1) < 0){
          printf("Can\'t wait for condition\n");
          exit(-1);
        }

        if(read(fd[0], resstring, 23) < 0){
          printf("Can\'t read string from pipe\n");
          exit(-1);
        }

        printf("%s", resstring);

        if(write(fd[1], "Get a ball from child\n", 22) != 22){
          printf("Can\'t write all string to pipe\n");
          exit(-1);
        }

        mybuf.sem_op = -1;
        
        if(semop(semid, &mybuf, 1) < 0){
          printf("Can\'t wait for condition\n");
          exit(-1);
        }

      }
      
      close(fd[1]);
      close(fd[0]);
      printf("Child exit\n");
   }

   return 0;
}
