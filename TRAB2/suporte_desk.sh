#!/bin/bash

# Validar o número de argumentos
if [ "$#" -ne 4 ]; then
    echo "Uso: $0 NALUN NDISCIP NLUG NSTUD"
    exit 1
fi

# Passo 1: Criar o named pipe /tmp/suporte
PIPE="/tmp/suporte"

# Verifica se o named pipe existe
if [ -p "$PIPE" ]; then
    rm "$PIPE"
fi
mkfifo "$PIPE"

# Passar argumentos
NALUN=$1   # Número máximo de alunos
NDISCIP=$2 # Número de disciplinas
NLUG=$3    # Número de lugares por sala
NSTUD=$4   # Número de processos student

# Passo 2 e 3: Calcular o número máximo de horários (NHOR)
NHOR=$((NALUN / NLUG))

#Passo 4: Executar o support_agent em background com o argumento NALUN
./support_agent "$NALUN" &


# Passo 5: Calcular o número de alunos a inscrever por cada processo student
NUM_ALUNOS_POR_SALA=$((NALUN / NSTUD))

echo "{
    1. Número máximo de alunos = $NALUN [recebido]
    2. Número de disciplinas = $NDISCIP [recebido]
    3. Número de lugares por sala = $NLUG [recebido]
    4. Número de processos student = $NSTUD [recebido]
    5. Número de horários = $NSTUD [calculado, (L1 / L3)]
    6. Alunos por sala = $NUM_ALUNOS_POR_SALA [calculado, (L1 / L4)]
}"


ALUNO_INICIAL=0

# Loop para iniciar os processos student
for ((i = 1; i <= NSTUD; i++)); do
    ./src/student "$i" "$ALUNO_INICIAL" "$NUM_ALUNOS_POR_SALA" &
    # ./src/student "$i" "$ALUNO_INICIAL" "$NUM_ALUNOS_POR_SALA"
    ALUNO_INICIAL=$((ALUNO_INICIAL + NUM_ALUNOS_POR_SALA))
done

# # Passo 6: Esperar até que todos os processos terminem
# wait

# # Passo 7: Remover o named pipe
# rm "$PIPE"

# exit 0
