#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
   int     fd_1[2];
   int     fd_2[2];
   int     result;

   size_t size;
   char  resstring[26];
   char  answer_string [12];

   if(pipe(fd_1) < 0){
     printf("Can\'t open pipe\n");
     exit(-1);
   }

   if(pipe(fd_2) < 0){
     printf("Can\'t open pipe\n");
     exit(-1);
   }

   result = fork();


   if(result < 0) {
      printf("Can\'t fork child\n");
      exit(-1);
   } else if (result > 0) {

     /* Parent process */

      close(fd_1[0]);
      close(fd_2[1]);

      size = write(fd_1[1], "Hello, what is your name?", 26);

      if(size != 26){
        printf("Can\'t write all string to pipe\n");
        exit(-1);
      }

      size = read(fd_2[0], answer_string, 12);

      if(size < 0){
         printf("Can\'t read string from pipe\n");
         exit(-1);
      }

      int name[1];

      size = read(fd_2[0], name, 4);

      if(size < 0){
         printf("Can\'t read string from pipe\n");
         exit(-1);
      }

      printf("Parent exit, resstring:%s %d.\n", answer_string, name[0]);

      close(fd_1[1]);
      close(fd_2[0]);
      printf("Parent exit\n");

   } else {

      /* Child process */

      close(fd_1[1]);
      close(fd_2[0]);

      size = read(fd_1[0], resstring, 26);

      if(size < 0){
         printf("Can\'t read string from pipe\n");
         exit(-1);
      }

      printf("Child exit, resstring:%s\n", resstring);


      size = write(fd_2[1], "My name is", 12);

      if(size != 12){
        printf("Can\'t write all string to pipe\n");
        exit(-1);
      }

      int name[1];
      name[0] = (int)getpid();

      size = write(fd_2[1], name, 4);

      if(size != 4){
        printf("Can\'t write all string to pipe\n");
        exit(-1);
      }

      close(fd_1[0]);
      close(fd_2[1]);
      printf("Child exit\n");
   }

   return 0;
}
