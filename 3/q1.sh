#!/bin/bash
if [ $# != 2 ]
then
	echo 'Invalid input number!'
	exit
fi

if [[ ! $1 =~ ^[0-9]+$ ]] || [[ ! $2 =~ ^[0-9]+$ ]]
then
echo 'invalid input!'
exit
fi


echo sum of numbers: $(($1+$2))
 

if [ $1 -gt $2 ]
then
echo maximum: $1
else
echo maximum: $2
fi

