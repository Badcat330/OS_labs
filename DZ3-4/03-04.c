#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

int main(int argc, char *argv[], char *envp[])
{
	printf("\nЗначения массива argv\n");
    for(int i = 0; i < argc; ++i)
    {
    	printf("%s \n", argv[i]);
    }
    printf("Значения массива envp\n");
    for (int i = 0; envp[i][0] != '0'; ++i)
    {
    	printf("%s \n", envp[i]);
    }

    return 0;

}