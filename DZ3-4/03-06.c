#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[], char *envp[])
{
    pid_t pid, ppid, chpid;
    int a = -1;

    chpid = fork();

    pid  = getpid();
    ppid = getppid();

    if(chpid == -1)
    {
    	printf("ОШИБКА \n");
    }
    else if(chpid == 0)
    {
    	printf("My pid = %d, my ppid = %d, result = %d\n", (int)pid, (int)ppid, a);
        execle("./03-01", "./03-01", 0, envp);
        printf("Error on program start\n");
        exit(-1);
    }
    else
    {
    	a = 1;
        printf("My pid = %d, my ppid = %d, result = %d\n\n", (int)pid, (int)ppid, a);
    }
    

    return 0;

}