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

    switch(chpid)
    {
        case -1:
            printf("Something go wrong\n");
            break;
        case 0:
            printf("My pid = %d, my ppid = %d, result = %d\n", (int)pid, (int)ppid, a);
            execle("/bin/cat", "/bin/cat", "03-05.c", 0, envp);
            printf("Error on program start\n");
            exit(-1);
            break;
        default:
            a = 1;
            printf("My pid = %d, my ppid = %d, result = %d\n\n", (int)pid, (int)ppid, a);
            break;
    }

    return 0;

}