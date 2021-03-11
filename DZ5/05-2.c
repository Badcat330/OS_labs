#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
   int     fd;
   size_t  size_read = 14;
   char    string[size_read];


   if((fd = open("myfile2", O_RDONLY)) < 0){
     printf("Can\'t open file\n");
     exit(-1);
   }

   size_read = read(fd, string, size_read);

   if(size_read != 14){
     printf("Can\'t read all string\n");
     exit(-1);
   }

   printf("%s\n", string);

   if(close(fd) < 0){
     printf("Can\'t close file\n");
   }

   return 0;
}
