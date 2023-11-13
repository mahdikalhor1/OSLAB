#!/bin/bash

read -p 'Enter number of begin line: ' begin
read -p 'Enter number of end line: ' end

if [[ ! $begin =~ ^[0-9]+$ ]] || [[ ! $end =~ ^[0-9]+$ ]]
then
echo 'Invalid input!'
exit
fi

read -p 'Enter file name: ' file_name

n_line=1
while read -r line
do
	if [ $n_line -gt $end ]
	then
	exit
	fi
	
	if [ $n_line -ge $begin ]
	then
		echo $line
	fi
	let n_line=n_line+1
done < $file_name
