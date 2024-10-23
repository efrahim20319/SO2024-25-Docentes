#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

#define BSIZE 80
#define NOMEFIFO "FIFO1"

char msg1[BSIZE];
char msg2[BSIZE];

int fd;
pid_t pid;

int main(int argc, char const *argv[])
{
    int n;

    if (mkfifo (NOMEFIFO, 0666) < 0) {
        perror ("mkfifo");
        exit(1);
    }

    if ((fd = open (NOMEFIFO, O_WRONLY)) < 0) {
        perror ("open");
        exit(1);
    }

    write (fd, msg1, sizeof(msg1));

    return 0;
}



