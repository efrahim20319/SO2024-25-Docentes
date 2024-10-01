#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>    // open
#include <sys/stat.h> // mkfifo
#include <unistd.h>   // write, close

int main(int argc, char *argv[]) {
    char *fifo_path ="/tmp/suporte";

    // Abre o named pipe para escrita
    int fd = open(fifo_path, O_WRONLY);
    if (fd == -1)
    {
        perror("Erro ao abrir o FIFO");
        exit(EXIT_FAILURE);
    }

    // Escreve dados no pipe
    char * message = argv[1]; 
    char * newMessage[strlen(argv[1]) + 1];
    strcpy(newMessage, message);
    strcat(newMessage, "\n");

    write(fd, newMessage, strlen(newMessage) + 1);

    //Fecha o arquivo
    close(fd);

    return 0;
}
