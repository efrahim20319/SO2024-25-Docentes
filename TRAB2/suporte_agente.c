#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>    // open
#include <sys/stat.h> // mkfifo
#include <unistd.h>   // write, close

int main(int argc, char const *argv[]) {
    char bufferPedido[256];

    if (argc != 2) {
        fprintf(stderr, "Uso: %s <numero_de_alunos>\n", argv[0]);
        return 1;
    }

    int max_alunos = atoi(argv[1]);
    int alunos_inscritos_total = 0;

    printf("Abrindo o pipe para leitura -> /tmp/suporte\n");
    // Abre o named pipe comum para receber pedidos
    int fdSuporte = open("/tmp/suporte", O_RDONLY);
    if (fdSuporte == -1) {
        perror("Erro ao abrir o pipe");
        return 1;
    }

    while (read(fdSuporte, bufferPedido, sizeof(bufferPedido)) > 0) {
        int aluno_inicial, num_alunos;
        char pipeResposta[50];

        // Extrai os dados do pedido
        sscanf(bufferPedido, "%d %d %s", &aluno_inicial, &num_alunos, pipeResposta);

        printf("Pedido recebido: aluno_inicial=%d, num_alunos=%d, pipeResposta=%s\n", aluno_inicial, num_alunos, pipeResposta);

        // Calcula o número de alunos que podem ser inscritos
        int alunos_inscritos = (alunos_inscritos_total + num_alunos <= max_alunos) ? num_alunos : max_alunos - alunos_inscritos_total;
        alunos_inscritos_total += alunos_inscritos;

        // Abre o pipe de resposta do student
        int fdResposta = open(pipeResposta, O_WRONLY);
        if (fdResposta == -1) {
            perror("Erro ao abrir o pipe de resposta do student");
            continue;
        }

        // Envia o número de alunos inscritos de volta ao student
        write(fdResposta, &alunos_inscritos, sizeof(alunos_inscritos));
        close(fdResposta);

        printf("Enviada resposta para %s: alunos_inscritos=%d\n", pipeResposta, alunos_inscritos);

        // Termina se todos os alunos tiverem sido inscritos
        if (alunos_inscritos_total >= max_alunos) {
            printf("Todos os alunos foram inscritos. Encerrando o agent.\n");
            break;
        }
    }

    close(fdSuporte);
    return 0;
}
