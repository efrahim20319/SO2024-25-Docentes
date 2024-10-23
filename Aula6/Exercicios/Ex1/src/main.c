#include <stdio.h>

int le_pipe (int fd_in, char *msg_in, int bsize)
{
    int j;
    for(j=0; j<bsize; j++) {
        if (read (fd_in, &msg_in[j], 1) < 0) {
            perror ("le_pipe read");
            exit(1);
        }
        if (msg_in[j] == '\0')
            break;
    }
    return j;
}


int main(int argc, char const *argv[])
{
    printf("Hello World!\n");

    return 0;
}
