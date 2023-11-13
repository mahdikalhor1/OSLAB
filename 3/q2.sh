#!/bin/bash

read -p "Enter first number : " first_number
read -p "Enter operand? (* / + -): " operand
read -p "Enter second number = " second_number

if [[ ! $first_number =~ ^[0-9]+$ ]] || [[ ! $second_number =~ ^[0-9]+$ ]]
then
echo 'Invalid input!'
exit
fi

echo result is:

case $operand in
	
	/)
	echo $(($first_number / $second_number))
	;;
		
	\*)
	echo $(($first_number * $second_number))
	;;
	
	+)
	echo $(($first_number + $second_number))
	;;
	
	-)
	echo $(($first_number - $second_number))	
	;;
esac
