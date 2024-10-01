#!bin/bash

sum=0
count=0
for i in "$@"; 
do
sum=$(($sum + $i))
count=$(($count + 1))
done
echo "$count"
echo " scale=2; $sum / $count" | bc
