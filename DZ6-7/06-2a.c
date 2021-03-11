#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>


int main()
{
	char     *array;
	int     shmid;
	char    pathname[] = "06-2a.c";
	key_t   key;
	int		fd;
	int		size_read;
	int 	size = 1052;

	if((key = ftok(pathname,0)) < 0){
    	printf("Can\'t generate key\n");
    	exit(-1);
    }

    if((shmid = shmget(key, size, 0666|IPC_CREAT)) < 0){
    	printf("Can\'t create shared memory\n");
    	exit(-1);
    }

    if((array = (char *)shmat(shmid, NULL, 0)) == (char *)(-1)){
		printf("Can't attach shared memory\n");
		exit(-1);
    }

    if((fd = open("06-2a.c", O_RDONLY)) < 0){
		printf("Can\'t open file\n");	
		exit(-1);
    }

    size_read = read(fd, array, size);
    
    if(size_read != size){
		printf("Can\'t read all file %d \n", size_read);
		exit(-1);
    }

    if(close(fd) < 0){
		printf("Can\'t close file\n");
    }

    if(shmdt(array) < 0){
		printf("Can't detach shared memory\n");
		exit(-1);
    }

	return 0;
}