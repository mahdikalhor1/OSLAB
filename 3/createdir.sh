#!/bin/bash

i=0

while [ $i -lt 50 ]
do
	mkdir newdir
	cd newdir
	let i=i+1
done
