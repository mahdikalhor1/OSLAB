#!/bin/bash

read -p 'enter a number between 1-3: ' input

case $input in
	
	1)
		
		for i in {1..5..1}
		do
			for (( j=0; j < i; j++))
			do
			echo -n $i
			done
			echo
		done
	;;
	2)
		
		for i in {1..6..1}
		do
			for (( j=0; j < 6 - i; j++))
			do
			echo -n ' '
			done
			
			for (( j=0; j < i; j++))
			do
			echo -n '. '
			done
			echo
			
		done
		
		for i in {6..1..-1}
		do
			for (( j=0; j < 6 - i; j++))
			do
			echo -n ' '
			done
			
			for (( j=0; j < i; j++))
			do
			echo -n '. '
			done
			echo
			
		done
	;;
	3)
		
		for i in {1..5..1}
		do
			for (( j=0; j < i; j++))
			do
			echo -n ' |'
			done
			echo '_'
		done
	;;
esac

