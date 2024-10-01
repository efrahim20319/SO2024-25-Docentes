#!/bin/bash​

#Verifica se script tem o argumento​
#Sera que falta um if?​
if [ $# -eq 0 ];
then​
  echo "Utilização: '$ $0 <palavra a procurar> <diretório>"
  exit 1
else
  echo "Argumentos"
  echo "==========================="
  echo "Numero de argumentos.: $#"
  echo "Lista de argumentos...: $@"
  echo "Arg #1..............: $1"
  echo "Arg #2..............: $2"
  echo "==========================="
fi