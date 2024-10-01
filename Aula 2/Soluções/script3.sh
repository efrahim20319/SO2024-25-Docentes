#!/usr/bin/bash​
#3.1​
echo "cat fich.txt"
read cont
cat fich.txt

#3.2​
echo "cat fich.txt > temp/fich2.txt"
read cont
mkdir temp
cat fich.txt > temp/fich2.txt

#3.3
echo "diff fich.txt temp/fich2.txt"
read cont
diff fich.txt temp/fich2.txt

#3.4​
echo "cat >> temp/fich2.txt "Hello World""
read cont
echo >> temp/fich2.txt "Hello World"
cat temp/fich2.txt

#3.5
echo "diff fich.txt temp/fich2.txt"
read cont
diff fich.txt temp/fich2.txt