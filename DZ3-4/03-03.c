#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

int main()
{
    pid_t pid, ppid, chpid;
    int a = -1;

    chpid = fork();

    pid  = getpid();
    ppid = getppid();

    if(chpid == -1)
    {
    	a = 0;
    }
    else if(chpid == 0)
    {
    	a = a + 1;
    }
    else
    {
    	a = 1;
    }
    printf("My pid = %d, my ppid = %d, result = %d\n", (int)pid, (int)ppid, a);

    return 0;

}