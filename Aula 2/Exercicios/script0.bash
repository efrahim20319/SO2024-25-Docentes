#!/bin/bash

echo "My Name Is vangence"

myName="Efrahim"

echo $myName

pwd

cmd="ls"

$cmd


echo $#

echo $$ # PID

echo "Say My Name: "
read mrWhite

echo "Mr White ($mrWhite) : You get that Right"

echo "Ditetorio a criar: "
read dir
mkdir temp

mkdir temp/$dir

ls temp

echo "Novo nome do diretorio"
read novoDir
mv "temp/$dir" "temp/$novoDir"
ls temp