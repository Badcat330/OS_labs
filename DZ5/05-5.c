#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[], char *envp[])
{
   int     fd[2], result;

   size_t size;
   char  resstring[270];

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

      close(fd[0]);

      if(dup2(fd[1], 1) < 0){
        printf("Can\'t redirect stdout\n");
        exit(-1);
      }

      (void) execle("/bin/cat", "/bin/cat", "../DZ3-4/03-05.c" ,0, envp);

      printf("Error on program start\n");

      close(fd[1]);
      printf("Parent exit\n");

   } else {

      /* Child process */

      close(fd[1]);
      size = read(fd[0], resstring, 270);

      if(size < 0){
         printf("Can\'t read string from pipe\n");
         exit(-1);
      }

      int fd_new;

      umask(0);
      if((fd_new = open("myfile3", O_WRONLY | O_CREAT, 0666)) < 0){
       printf("Can\'t open file\n");
       exit(-1);
      }

      if(dup2(fd_new, 1) < 0){
        printf("Can\'t redirect stdout\n");
        exit(-1);
      }

      (void) execle("/bin/echo", "/bin/echo", resstring ,0, envp);

      printf("Error on program start\n");

      close(fd[0]);
   }

   return 0;
}
