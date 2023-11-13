#!/bin/bash

while true
do
	
	read -p 'enter a number: (enter -1 to exit) ' input
	
	if [ $input == -1 ]
	then
		exit
	fi
	
	if [[ $input =~ ^[0-9]+$ ]]
	then
		sum=0
		reverse=0
		
		while [ $input != 0 ]
		do 
			let reminder=input%10
			let sum=sum+reminder
			let reverse=reverse*10+reminder
			let input=input-reminder
			let input=input/10
			
		done
		
		echo 'reversed number : ' $reverse
		echo 'sum of digits : ' $sum
	else
		echo 'Invalid input!'
	fi
	
	
done
