#!/usr/bin/bash​
#script1: if's​

echo "Nome="
read nome
echo "Nome=$nome"

if [ "$nome" == "$1" ]; then
    echo "OK: $nome = $1"
else
    echo "NOK: $nome != $1"
fi


# segunda versão
echo "Nome="
read nome
echo "Idade="

read idade
echo "Nome=$nome, Idade=$idade"

if [ "$nome" == "$1" ] && [ "$idade" -le $2 ]; then
    echo "OK: $nome == $1 && $idade < $2"
else
    echo "NOK: $nome != $1 || $idade >= $2"
fi