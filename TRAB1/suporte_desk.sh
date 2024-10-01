#!/bin/bash

# Criar o named pipe (FIFO)
mkfifo "/tmp/suporte"

# Iniciar o suporte_agente.sh em background para ler do named pipe
./suporte_agente.sh &

# Aguardar um pouco para garantir que suporte_agente.sh esteja pronto
sleep 1

# Enviar mensagens para o named pipe
./src/main "Bom Dia, o meu horario nao esta correto"
./src/main "Nao eh problema nosso, tchau"
./src/main exit

# O suporte_agente.sh deve estar lendo as mensagens agora
# Remover o named pipe quando terminar
rm "/tmp/suporte"

exit 0