#!/bin/bash
#Laboratory work 3
#Baranets Maxim
#Variant 3

if [[ $# -eq 0 ]]; then echo "err: no args" >&2
	exit 1
fi

if [[ !( -f "$1" ) ]]; then 
	echo "err: $1 isn't a regular file" >&2
	exit 1
fi


file_info=$(ls -ld "$1" | awk '{print $1":"$3}')
perm=${file_info%:*}
owner=${file_info#*:}
uid_gid=$(getent passwd | grep "$owner" | cut -d: -f 3,4 )

regex="[^:]+:[^:]+:[^:]+:${uid_gid#*:}:*"

if [[ -n $(echo $perm | egrep -e "-..x......") ]]; then
	echo $owner 
fi

if [[ -n $(echo $perm | egrep -e "-.....x...") ]]; then
	getent passwd | egrep -v "^$owner" | egrep "$regex" | cut -d: -f 1  
fi

if [[ -n $(echo $perm | egrep -e "-........x") ]]; then
	getent passwd | egrep -v "$regex" | cut -d: -f 1 
fi