#!/bin/bash

fifo_path="/tmp/suporte"

# Verifica se o named pipe existe
if [ ! -p "$fifo_path" ]; then
    echo "Nao ha pipe"
    exit 1
fi

while true; do
    if read line; then
        if [ "$line" = "exit" ]; then
            echo "Encerrando o loop."
            exit 0
        fi
        echo "Mensagem recebida: $line"
        sleep 1
    fi
done <"$fifo_path"
