#!/bin/ksh
#Baranets Maxim
#Variant 4

tmpIFS=$IFS
IFS=$':'
SCRIPTLOGS="$PWD/logs.txt"

if [ ! -f "$SCRIPTLOGS" ]; then 
        touch "$SCRIPTLOGS"
fi    

if [ -n "$1" ]; then
        print "Wrong start option. Please try again without input arguments!"
else
        set -A my_path $PATH
		#gfind (Helios/Oracle Solaris) = find (MacOS)
        var1=`find -L ${my_path[@]} -type f -executable -printf "%P\n" 2>>$SCRIPTLOGS`
        var2=`find . -perm -100 | sed 1d | sed 's/.\///g'`
        print "${var2[@]}\n ${var1[@]}"| sort -r | head
fi

IFS=$tmpIFS