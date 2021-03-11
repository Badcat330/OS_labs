#include <dirent.h>
#include <stdio.h>
#include <sys/stat.h>
#include <string.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <stdlib.h>

char *formatdate(char *str, time_t val)
{
    strftime(str, 36, "%d %b %H:%M", localtime(&val));
    return str;
}

int main(int argc, char *argv[])
{
    DIR *d;
    struct dirent *dir;
    struct stat buffer;
    char buf[1024];
    struct passwd *pwd;
    struct group *grp;
    char date[36];

    if (argc == 1)
    {
        d = opendir(".");
    }
    else
    {
        d = opendir(argv[1]);
    }

    if (d)
    {
        while ((dir = readdir(d)) != NULL)
        {
            memset(&buf[0], 0, sizeof(buf));
            if (argc == 1)
            {
                strcat(buf, ".");
            }
            else
            {
                strcat(buf, argv[1]);
            }
            strcat(buf, "/");
            strcat(buf, dir->d_name);

            if (lstat(buf, &buffer) == -1)
            {
                printf("%s\n", "Can't read file stats");
                return -1;
            }
            else
            {
                pwd = getpwuid(buffer.st_uid);
                if (pwd == NULL)
                {
                    printf("%s\n", "Get user name error");
                    return -1;
                }

                grp = getgrgid(buffer.st_gid);
                if (grp == NULL)
                {
                    printf("%s\n", "Get group name error");
                }

                printf((S_ISDIR(buffer.st_mode)) ? "d" : "-");
                printf((buffer.st_mode & S_IRUSR) ? "r" : "-");
                printf((buffer.st_mode & S_IWUSR) ? "w" : "-");
                printf((buffer.st_mode & S_IXUSR) ? "x" : "-");
                printf((buffer.st_mode & S_IRGRP) ? "r" : "-");
                printf((buffer.st_mode & S_IWGRP) ? "w" : "-");
                printf((buffer.st_mode & S_IXGRP) ? "x" : "-");
                printf((buffer.st_mode & S_IROTH) ? "r" : "-");
                printf((buffer.st_mode & S_IWOTH) ? "w" : "-");
                printf((buffer.st_mode & S_IXOTH) ? "x" : "-");
                printf(" %d", buffer.st_nlink);
                printf(" %s", pwd->pw_name);
                printf(" %s", grp->gr_name);
                printf("% lld", buffer.st_size);
                printf(" %s", formatdate(date, buffer.st_ctime));
                printf(" %s\n", dir->d_name);
            }
        }
        closedir(d);
    }
    else
    {
        printf("%s\n", "Incorrect path to file");
    }

    return (0);
}