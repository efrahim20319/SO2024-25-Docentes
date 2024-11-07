#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

#define BSIZE 80
#define NOMEFIFO "/tmp/suporte"

char msg1[BSIZE] = "5";
int fd;
pid_t pid;
int vagas = 100;

void* preenche_vagas(void * p)
{
    int *preenche = (int*) p;
    vagas = vagas - (*preenche);
}

int le_pipe (int fd, char *msg_in, int bsize)
{
    int j;
    for(j=0; j<bsize; j++) msg_in[j]=0;
    for(j=0; j<bsize; j++) {
        if (read (fd, &msg_in[j], 1) < 0) {
            perror ("le_pipe read");
            exit(1);
        }
        if (msg_in[j] == '\0')
            break;
    }
    return j;
}

/**
 * Programa que le do pipe
 */
int main(int argc, char const *argv[])
{
    printf("Vagas inicio = %d", vagas);
    int dados = 0;
    if (mkfifo (NOMEFIFO, 0666) < 0) {
        perror ("mkfifo");
        printf ("Fifo ja existe\n"); 
    }

    if ((fd = open (NOMEFIFO, O_RDONLY)) < 0) {
        perror ("open");
        exit(1);
    }

    //read (fd, msg1, sizeof(msg1));
    le_pipe (fd, msg1, sizeof(msg1));
    printf ("Recebi: %s\n", msg1);
    dados = atoi(msg1);
    preenche_vagas(&dados);


    printf("Vagas fim = %d", vagas);

    close(fd);
    return 0;
}
