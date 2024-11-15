#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pthread.h>

typedef struct
{
    int aluno_inicial;
    int num_alunos;
    char pipeResposta[50];
} Pedido;

int le_pipe(int fd, char *msg_in, int bsize)
{
    int j;
    for (j = 0; j < bsize; j++)
        msg_in[j] = 0;
    for (j = 0; j < bsize; j++)
    {
        if (read(fd, &msg_in[j], 1) < 0)
        {
            perror("le_pipe read");
            exit(1);
        }
        if (msg_in[j] == '\0')
            break;
    }
    return j;
}

void *processa_pedido(void *arg)
{
    Pedido *pedido = (Pedido *)arg;
    int alunos_inscritos = 5;

    printf("Thread para aluno_inicial=%d a processar pedido. Pipe de resposta: %s\n",
           pedido->aluno_inicial, pedido->pipeResposta);


    int fdResposta = open(pedido->pipeResposta, O_WRONLY);
    if (fdResposta == -1)
    {
        perror("Erro ao abrir o pipe de resposta do student");
        free(pedido);
        pthread_exit(NULL);
    }

    // Envia o número de alunos inscritos de volta ao student
    if (write(fdResposta, &alunos_inscritos, sizeof(alunos_inscritos)) == -1)
    {
        perror("Erro ao enviar a resposta ao student");
    }
    close(fdResposta);
    printf("Enviada resposta para %s: alunos_inscritos=%d\n", pedido->pipeResposta, alunos_inscritos);

    free(pedido);
    pthread_exit(NULL);
}

int main(int argc, char const *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Uso: %s <numero_de_alunos>\n", argv[0]);
        return 1;
    }

    int max_alunos = atoi(argv[1]);
    char bufferPedido[256];

    printf("Abrindo o pipe para leitura -> /tmp/suporte\n");
    int fdSuporte = open("/tmp/suporte", O_RDONLY);
    if (fdSuporte == -1)
    {
        perror("Erro ao abrir o pipe");
        return 1;
    }

    while (le_pipe(fdSuporte, bufferPedido, sizeof(bufferPedido)) > 0)
    {
        Pedido *pedido = malloc(sizeof(Pedido));
        if (pedido == NULL)
        {
            perror("Erro ao alocar memória para o pedido");
            continue;
        }

        sscanf(bufferPedido, "%d %d %s", &pedido->aluno_inicial, &pedido->num_alunos, pedido->pipeResposta);
        printf("Pedido recebido: aluno_inicial=%d, num_alunos=%d, pipeResposta=%s\n",
               pedido->aluno_inicial, pedido->num_alunos, pedido->pipeResposta);

        pthread_t tid;
        if (pthread_create(&tid, NULL, processa_pedido, (void *)pedido) != 0)
        {
            perror("Erro ao criar thread para o pedido");
            free(pedido);
        }

        if (pthread_join(tid, NULL) != 0)
        {
            printf("Fck hll\n");
            return 1;
        }
    }

    close(fdSuporte);
    return 0;
}
