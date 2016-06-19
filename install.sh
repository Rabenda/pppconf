#!/bin/sh
if [ -f "pppconf" ]
then
	cp "pppconf" "/bin/pppconf"
	echo "pppconf installed succefully"	
else
	echo "First, execute sh build.sh"
fi
