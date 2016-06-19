#!/bin/sh
echo "Compiling..."
gcc -Wall -o pppconf pppmain.c
if [ -f "pppconf" ]
then
	echo "OK, now execute sh install.sh"
else
	echo "Failed to compile pppconf"
fi
