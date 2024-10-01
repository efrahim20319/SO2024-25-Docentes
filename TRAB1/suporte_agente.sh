#!/bin/bash

fifo_path="/tmp/suporte"  # Substitua o caminho para /tmp

# Verifica se o named pipe existe
if [ ! -p "$fifo_path" ]; then
    echo "Nao ha pipe"
    exit 1
fi

# Loop para ler continuamente do named pipe
while true; do
    if read line; then
        if [ "$line" = "exit" ]; then  # Corrigido para usar "="
            echo "Encerrando o loop."
            exit 0  # Saída normal
        fi
        echo "Mensagem recebida: $line"  # Exibe a mensagem recebida
    fi
    sleep 1  # Pausa de 1 segundo antes de continuar no loop
done < "$fifo_path"
