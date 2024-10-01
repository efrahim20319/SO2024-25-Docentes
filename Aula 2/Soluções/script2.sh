#!/usr/bin/bash​
#2.1
echo "for ((a=1; a <= 3; a++))"
read xx

for ((a=1; a <= 3; a++))
do
    echo $a
done

#2.2​
echo "for Variable in {5..7}"
read xx

for Variable in {5..7}
do
    echo "$Variable"
done

#2.3​
echo "for Variable in {a..f}"
read xx

for Variable in {a..f}
do
    echo "$Variable"
done

#2.4
echo "for Variable in" '$ls'
read xx
for Variable in $(ls)
do
    echo "$Variable"
done