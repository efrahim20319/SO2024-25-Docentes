#!/bin/bash


#0.1 echo
echo "------------------------"
echo "Olá Mundo!" # Belo do Hello world
s="Hello World!"
echo $s

#0.2 Executar comandos
echo "------------------------"
pwd
cmd="ls"
$cmd

#0.3 Built-in variables
echo "------------------------"
echo "Número de argumentos passados ao script: $#"
echo "Todos os argumentos são: $@"
echo "Os argumentos do scripts listados individualmente: $1 $2 $3"
echo "Valor retornado pelo ultimo programa: $?"
echo "Numero do processo do script (PID): $$"

#0.4 Ler um valor da linha de comando
echo "------------------------"
echo "Como te chamas?"
read nome
echo "Olá, $nome!"
echo "Escreve um comando linux "
read comando
$comando

#0.5 Combinar tudo
echo "Que pasta quer criar?"
read pasta
mkdir temp
mkdir temp/$pasta
ls temp
