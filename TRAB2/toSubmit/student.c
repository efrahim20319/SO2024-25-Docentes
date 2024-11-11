#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>    // open
#include <sys/stat.h> // mkfifo
#include <unistd.h>   // write, close

int main(int argc, char *argv[]) {
    char *pipeSuporte ="/tmp/suporte";

    if (argc != 4) {
        fprintf(stderr, "Formato de iniciação errado.\n Exemplo: ./student <NSTUD> <ALUNO_iNICIAL> <ALUNOS_POR_INSCREVER>");
        exit(1);
    }

    // Lê argumentos passados na execução do programa
    int student_id = atoi(argv[1]);
    int aluno_inicial = atoi(argv[2]);
    int num_alunos = atoi(argv[3]);
    int alunos_inscritos = 0;
    char pedido[256];
    char pipeResposta[50] = "/tmp/student_";

    snprintf(pipeResposta, sizeof(pipeResposta), "/tmp/student_%d", student_id);
    mkfifo(pipeResposta, 0666);

    // Mensagem inicial
    printf("student %d: aluno inicial=%d, número de alunos=%d\n", student_id, aluno_inicial, num_alunos);


    snprintf(pedido, sizeof(pedido), "%d %d %s", aluno_inicial, num_alunos, pipeResposta);
    puts(pedido);

    // // Abre o named pipe para escrita
    // int fd = open(pipeSuporte, O_WRONLY);
    // if (fd == -1)
    // {
    //     perror("Erro ao abrir o FIFO");
    //     exit(EXIT_FAILURE);
    // }

    // // Escreve dados no pipe
    // char * newMessage[strlen(pedido) + 1];
    // strcpy(newMessage, pedido);
    // strcat(newMessage, "\n");
    // write(fd, newMessage, strlen(newMessage) + 1);

    // //Fecha o arquivo
    // close(fd);

    return 0;
}
