#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/sem.h>

int main(void)
{

    int fd;
    size_t length;
    int i;
    int semid;
    char pathname[] = "../11-3a.c";
    key_t key;
    struct sembuf mybuf;

    struct A
    {
        int message;
    } * ptr, *tmpptr;

    fd = open("share.dat", O_RDWR | O_CREAT, 0666);

    if (fd == -1)
    {
        printf("File open failed!\n");
        exit(1);
    }

    length = sizeof(struct A);

    ftruncate(fd, length);

    ptr = (struct A *)mmap(NULL, length, PROT_WRITE | PROT_READ,
                           MAP_SHARED, fd, 0);

    close(fd);

    if (ptr == MAP_FAILED)
    {
        printf("Mapping failed!\n");
        exit(2);
    }

    if ((key = ftok(pathname, 0)) < 0)
    {
        printf("Can\'t generate key\n");
        exit(-1);
    }

    if ((semid = semget(key, 1, 0666 | IPC_CREAT)) < 0)
    {
        printf("Can\'t create semaphore set\n");
        exit(-1);
    }

    for (i = 1; i <= 100; i++)
    {
        mybuf.sem_num = 0;
        mybuf.sem_op = -1;
        mybuf.sem_flg = 0;

        if (semop(semid, &mybuf, 1) < 0)
        {
            printf("Can\'t wait for condition\n");
            exit(-1);
        }

        tmpptr = ptr;
        tmpptr->message += 1;
        printf("%d\n", tmpptr->message);

        mybuf.sem_op = -1;
        if (semop(semid, &mybuf, 1) < 0)
        {
            printf("Can\'t wait for condition\n");
            exit(-1);
        }
    }

    munmap((void *)ptr, length);
    return 0;
}