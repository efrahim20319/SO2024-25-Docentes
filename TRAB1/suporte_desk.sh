#!/bin/bash

mkfifo "/tmp/suporte"

# Inicia o script do agente em segundo plano
./suporte_agente.sh &

# Dá um tempo para o suporte_agente.sh iniciar
sleep 1

# Lista de mensagens a serem enviadas
mensagens=("Bom Dia, o meu horário não está correto" "Não é problema nosso, tchau" "exit")

# Loop através de cada mensagem, com índice
for i in "${!mensagens[@]}"; do
    echo "Iteração $i: ${mensagens[$i]}"
    ./src/student "${mensagens[$i]}"
done

# Aguarda todos os processos em segundo plano terminarem
wait

# Remove o FIFO
rm "/tmp/suporte"

exit 0
