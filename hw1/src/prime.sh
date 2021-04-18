#!/bin/bash

echo "INPUT FIRST NUMBER : "
read num1
echo "INPUT SECOND NUMBER : "
read num2


declare -i i j
declare -i cnt
declare -i sum
sum=0
for((i=$num1; i<=$num2; i++))
do
	cnt=0
	for((j=2; j<$i; j++))
	do
		if(($i%$j==0))
		then
			cnt=$cnt+1
		fi
	done
	
	if((cnt==0))
	then
		if(($i!=1))
		then
			sum=$sum+$i
		fi
	fi
done 
echo "SUM OF PRIME NUMBERS($num1 to $num2) : $sum"
