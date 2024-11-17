#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pthread.h>

#define MAX_HORARIOS 100
#define MAX_THREADS 500

typedef struct
{
    int lugares;
    int vagas;
} Horario;

typedef struct
{
    Horario horarios[MAX_HORARIOS];
} Disciplina;

typedef struct
{
    int aluno_inicial;
    int num_alunos;
    char pipeResposta[50];
} Pedido;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

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

void verificarHorariosCheios(Disciplina *disciplinas, Pedido *pedido, int numero_de_disciplinas, int numero_de_horarios)
{
    int todos_cheios = 1;

    for (int i = 0; i < numero_de_disciplinas && todos_cheios; i++)
    {
        for (int j = 0; j < numero_de_horarios; j++)
        {
            if (disciplinas[i].horarios[j].vagas > 0)
            {
                todos_cheios = 0;
                break;
            }
        }
    }

    if (todos_cheios)
    {
        printf("\033[1;31mTodos os horários estão cheios. Encerrando o programa.\033[0m\n");
        
        exit(0);
    }
}

void *processa_pedido(void *arg)
{
    struct
    {
        Pedido *pedido;
        Disciplina *disciplinas;
        int numero_de_disciplinas;
        int numero_de_horarios;
    } *data = arg;

    Pedido *pedido = data->pedido;
    Disciplina *disciplinas = data->disciplinas;
    int numero_de_disciplinas = data->numero_de_disciplinas;
    int numero_de_horarios = data->numero_de_horarios;

    int alunos_inscritos = 0;
    int alunos_para_increver = pedido->num_alunos;

    for (int i = 0; i < numero_de_disciplinas; i++)
    {
        for (int j = 0; j < numero_de_horarios; j++)
        {
            pthread_mutex_lock(&mutex);
            verificarHorariosCheios(disciplinas, pedido, numero_de_disciplinas, numero_de_horarios);

            if (disciplinas[i].horarios[j].vagas > 0 && alunos_para_increver > 0)
            {
                if (disciplinas[i].horarios[j].vagas - alunos_para_increver >= 0)
                {
                    disciplinas[i].horarios[j].vagas -= alunos_para_increver;
                    alunos_inscritos++;
                    printf("\033[1;33mDisciplina %d, Horario %d, Vagas restantes: %d\033[0m\n", i, j, disciplinas[i].horarios[j].vagas);
                    pthread_mutex_unlock(&mutex);

                    break; // Passa para a próxima disciplina
                }
                else
                {
                    disciplinas[i].horarios[j].vagas = 0;
                    alunos_inscritos++;
                    printf("\033[1;33mDisciplina %d, Horario %d, Vagas restantes: %d\033[0m\n", i, j, disciplinas[i].horarios[j].vagas);
                    pthread_mutex_unlock(&mutex);
                    break; // Passa para a próxima disciplina
                }

                //  OUTRA LOGICA
                // disciplinas[i].horarios[j].vagas--;
                // alunos_inscritos++;
                // alunos_para_increver--;
                // printf("\033[1;33mDisciplina %d, Horario %d, Vagas restantes: %d\033[0m\n", i, j, disciplinas[i].horarios[j].vagas);
                // pthread_mutex_unlock(&mutex);

                break; // Passa para a próxima disciplina
            }
            pthread_mutex_unlock(&mutex);
        }
    }

    // printf("Pedido processado: alunos_inscritos=%d para aluno_inicial=%d\n", alunos_inscritos, pedido->aluno_inicial);

    int fdResposta = open(pedido->pipeResposta, O_WRONLY);
    if (fdResposta == -1)
    {
        perror("Erro ao abrir o pipe de resposta");
        free(data);
        pthread_exit(NULL);
    }

    if (write(fdResposta, &alunos_inscritos, sizeof(alunos_inscritos)) == -1)
    {
        perror("Erro ao enviar a resposta");
    }
    close(fdResposta);

    free(data);
    pthread_exit(NULL);
}

void iniciarDisciplinas(Disciplina *disciplinas, int numero_de_disciplinas, int numero_de_horarios, int numero_de_lugares)
{
    for (int i = 0; i < numero_de_disciplinas; i++)
    {
        for (int j = 0; j < numero_de_horarios; j++)
        {
            disciplinas[i].horarios[j].lugares = numero_de_lugares;
            disciplinas[i].horarios[j].vagas = numero_de_lugares;
        }
    }
    // printf("Disciplinas e horários inicializados com %d lugares por horário.\n", numero_de_lugares);
}

int main(int argc, char const *argv[])
{
    if (argc != 6)
    {
        fprintf(stderr, "Uso: %s <numero_de_alunos> <numero_de_horarios> <numero_de_lugares> <numero_de_disciplinas>\n", argv[0]);
        return 1;
    }

    int numero_de_alunos = atoi(argv[1]);
    int numero_de_horarios = atoi(argv[2]);
    int numero_de_lugares = atoi(argv[3]);
    int numero_de_disciplinas = atoi(argv[4]);

    Disciplina disciplinas[numero_de_disciplinas];
    iniciarDisciplinas(disciplinas, numero_de_disciplinas, numero_de_horarios, numero_de_lugares);

    // printf("Abrindo o pipe para leitura -> /tmp/suporte\n");
    int fdSuporte = open(argv[5], O_RDONLY);
    if (fdSuporte == -1)
    {
        perror("Erro ao abrir o pipe");
        return 1;
    }

    char bufferPedido[256];

    pthread_t threads[MAX_THREADS];
    int thread_count = 0;
    sleep(3);
    while (le_pipe(fdSuporte, bufferPedido, sizeof(bufferPedido)) > 0)
    {
        Pedido *pedido = malloc(sizeof(Pedido));
        if (pedido == NULL)
        {
            perror("Erro ao alocar memória para o pedido");
            continue;
        }

        sscanf(bufferPedido, "%d %d %s", &pedido->aluno_inicial, &pedido->num_alunos, pedido->pipeResposta);

        struct
        {
            Pedido *pedido;
            Disciplina *disciplinas;
            int numero_de_disciplinas;
            int numero_de_horarios;
        } *data = malloc(sizeof(*data));

        if (data == NULL)
        {
            perror("Erro ao alocar memória para dados da thread");
            free(pedido);
            continue;
        }

        data->pedido = pedido;
        data->disciplinas = disciplinas;
        data->numero_de_disciplinas = numero_de_disciplinas;
        data->numero_de_horarios = numero_de_horarios;
        if (pthread_create(&threads[thread_count], NULL, processa_pedido, data) != 0)
        {
            perror("Erro ao criar thread");
            free(pedido);
            free(data);
        }
        else
        {
            pthread_join(threads[thread_count], NULL);
        }
        thread_count++;
        if (thread_count == MAX_THREADS)
        {
            printf("Limite de Threads excedido!!!\n\n\n");
            for (int i = 0; i < thread_count; i++)
            {
                pthread_join(threads[i], NULL);
            }
            thread_count = 0;
        }
    }
    for (int i = 0; i < thread_count; i++)
    {
        pthread_join(threads[i], NULL);
    }

    close(fdSuporte);
    pthread_mutex_destroy(&mutex);
    return 0;
}
