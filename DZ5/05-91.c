#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
   int     fd;
   size_t  size;
   char    resstring[14];
   char    name[] = "05-9.fifo";

    if((fd = open(name, O_RDONLY)) < 0){
       printf("Can\'t open FIFO for reading\n");
	     exit(-1); 
    }

    size = read(fd, resstring, 14);
    if(size < 0){
       printf("Can\'t read string from FIFO\n");
       exit(-1);
    }

    printf("%s\n", resstring);

    close(fd);

   return 0;
}
