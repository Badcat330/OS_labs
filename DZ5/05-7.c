#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
   const int  maxValue = 2147483;
   int  fd[2];
   int count = 0;
   size_t size;
   char  resstring[maxValue];
   char garbage[maxValue];

   if(pipe(fd) < 0){
     printf("Can\'t open pipe\n");
     exit(-1);
   }

   if(fcntl(fd[1], F_SETFL, O_NONBLOCK) < 0)
   {
      printf("Fcntl error \n");
      exit(-1);
   }

   for (int i = 0; i < maxValue; ++i)
   {
     resstring[i] = 'a';
     size = write(fd[1], resstring, i + 1);

     if(size != i + 1){
        printf("Result = %d \n", i);
        exit(-1);
      }

      size = read(fd[0], garbage, i + 1);

      if(size < 0){
         printf("Can\'t read string from pipe\n");
         exit(-1);
      }
   }

   
   close(fd[0]);
   close(fd[1]);
   return 0;
}
