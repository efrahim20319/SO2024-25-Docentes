#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

#define BSIZE 80
#define NOMEFIFO "/tmp/suporte"

char msg1[BSIZE] = "teste 1";

int fd;
pid_t pid;

int main(int argc, char const *argv[])
{
    if(argc > 1)
    {
        strcpy(msg1, argv[1]);
    }

    if ((fd = open (NOMEFIFO, O_WRONLY)) < 0) {
        perror ("open");
        exit(1);
    }

    strcat(msg1, "\n");
    write (fd, msg1, sizeof(msg1));

    sleep(1);
    close(fd);
    return 0;
}
