#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>    // open
#include <sys/stat.h> // mkfifo
#include <unistd.h>   // write, close

int main(int argc, char *argv[]) {
    char *pipeSuporte = "/tmp/suporte";

    if (argc != 4) {
        fprintf(stderr, "Formato de iniciação errado.\n Exemplo: ./student <NSTUD> <ALUNO_INICIAL> <ALUNOS_POR_INSCREVER>");
        exit(1);
    }

    // Lê argumentos passados na execução do programa
    int student_id = atoi(argv[1]);
    int aluno_inicial = atoi(argv[2]);
    int num_alunos = atoi(argv[3]);
    int alunos_inscritos = 0;
    char pedido[256];
    char pipeResposta[50];

    snprintf(pipeResposta, sizeof(pipeResposta), "/tmp/student_%d", student_id);
    mkfifo(pipeResposta, 0666);

    // Mensagem inicial
    printf("student %d: aluno inicial=%d, número de alunos=%d\n", student_id, aluno_inicial, num_alunos);

    // Formata o pedido
    snprintf(pedido, sizeof(pedido), "%d %d %s", aluno_inicial, num_alunos, pipeResposta);

    printf("Abrindo o pipe para escrita -> pipe %s\n", pipeSuporte);
    // Abre o named pipe para escrita
    int fdSuporte = open(pipeSuporte, O_WRONLY);
    if (fdSuporte == -1) {
        perror("Erro ao abrir o FIFO");
        exit(EXIT_FAILURE);
    }

    // Envia o pedido para o support_agent
    write(fdSuporte, pedido, strlen(pedido) + 1); // Inclui o terminador nulo

    // Fecha o named pipe de suporte
    close(fdSuporte);

    // Abre o pipeResposta para ler a resposta do support_agent
    int fdResposta = open(pipeResposta, O_RDONLY);
    if (fdResposta == -1) {
        perror("Erro ao abrir o pipe de resposta");
        unlink(pipeResposta);
        exit(EXIT_FAILURE);
    }

    // Lê o número de alunos efetivamente inscritos
    if (read(fdResposta, &alunos_inscritos, sizeof(alunos_inscritos)) > 0) {
        printf("\033[32mstudent %d: alunos inscritos=%d\033[0m\n", student_id, alunos_inscritos);
    } else {
        perror("Erro ao ler a resposta do support_agent");
    }

    // Limpeza final
    close(fdResposta);
    unlink(pipeResposta);

    return 0;
}
