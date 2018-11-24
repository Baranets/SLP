#!/bin/ksh
#Baranets Maxim
#Variant 4

count=1
if [ -n "$1" ]; then
    
        while  [ -n "$1" ]
        do
                if [[ $1 = +([0-9]) ]]; then
                        echo "Inputed count of processes: $1 - equal or greater processes started by users:"
                        users=`ps -Ao user | sed 1d | sort | uniq -c`
                        #nawk (Helios/Oracle Solaris) = awk (MacOS)
                        echo "${users[@]}" | awk -v count=$1 '{ if ($1 >= count) print $2}'
                else
                        echo "Wrong argument  №$count = $1: Script aimed to work with positive numbers."
                fi
                ((count=count+1))
                shift
        done        
else        
        print "Wrong start option. Please try again with argument counts of processes"
fi
