#!/bin/bash



if [ "$#" -ne 5 ]; then
    echo "Uso: $0 NALUN NDISCIP NLUG NSTUD PIPE_PATH"
    exit 1
fi


NALUN=$1        
NDISCIP=$2      
NLUG=$3         
NSTUD=$4        
PIPE=$5         


# Verificar e criar o named pipe se não existir
if [ -p "$PIPE" ]; then
    echo "Named pipe já existe: $PIPE. Removendo e recriando."
    rm "$PIPE"
fi
mkfifo "$PIPE"

# Passo 2 e 3: Calcular o número máximo de horários (NHOR)
NHOR=$((NALUN / NLUG))

# Passo 4: Executar o support_agent em background
./support_agent "$NALUN" "$NHOR" "$NLUG" "$NDISCIP" "$PIPE" &

# Passo 5: Calcular o número de alunos por processo student
INSCRICOES_POR_STUDENT=$((NALUN / NSTUD))

echo "{
    1. Número máximo de alunos = $NALUN [recebido]
    2. Número de disciplinas = $NDISCIP [recebido]
    3. Número de lugares por sala (vagas) = $NLUG [recebido]
    4. Número de processos student = $NSTUD [recebido]
    5. Número de horários = $NHOR [calculado, (L1 / L3)]
    6. Inscrições por student = $INSCRICOES_POR_STUDENT [calculado, (L1 / L4)]
    7. Pipe utilizado = $PIPE
}"

ALUNO_INICIAL=0

# Loop para iniciar os processos student
for ((i = 1; i <= NSTUD; i++)); do
    ./src/student "$i" "$ALUNO_INICIAL" "$INSCRICOES_POR_STUDENT" &
    ALUNO_INICIAL=$((ALUNO_INICIAL + INSCRICOES_POR_STUDENT))
done

# Passo 6: Esperar até que todos os processos terminem
wait

# Passo 7: Remover o named pipe
if [ -p "$PIPE" ]; then
    rm "$PIPE"
fi

exit 0
