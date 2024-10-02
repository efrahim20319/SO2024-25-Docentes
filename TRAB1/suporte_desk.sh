#!/bin/bash

mkfifo "/tmp/suporte"


./suporte_agente.sh &


sleep 1


./src/main "Bom Dia, o meu horario nao esta correto"
./src/main "Nao eh problema nosso, tchau"
./src/main exit

wait

rm "/tmp/suporte"

exit 0