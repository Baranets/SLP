#!/bin/bash
#Laboratory work 3
#Baranets Maxim
#Variant 4

if [[ $# -eq 0 ]]; then
	echo "err: no args" >&2
	exit 1
fi

if [[ !( -d "$1" ) ]]; then
	echo "err: $1 isn't dir" >&2
	exit 1
fi

dir_info=$(ls -ld "$1" | awk '{print $1":"$3":"$4}')
group=${dir_info#*:}
owner=${group%%:*}
group=${group#*:}
perm=${dir_info%%:*}
uid_gid=$(getent passwd | grep "$owner" | cut -d: -f 3,4 )

regex="[^:]+:[^:]+:[^:]+:${uid_gid#*:}:*"

if [[ -n $(echo $perm | egrep "d....w....") ]]; then
	if [[ $group = studs ]]; then 
		getent passwd | egrep "$regex" | egrep -v "$owner" | awk -F: '{print $1}' 	
	else 
		getent group | egrep "^$group:" | awk -F: '{print $4}' | tr , '\n' | egrep -v "$owner"
	fi
fi

if [[ -n $(echo $perm | egrep "d.w.......") ]]; then 
	echo $owner 
fi

if [[ -n $(echo $perm | egrep "d.......w.") ]]; then
	if [[ $group = studs ]]; then 
		getent passwd | egrep -v "$regex" | awk -F: '{print $1}' 	
	else 
		getent group | egrep -v "^$group:" | awk -F: '{print $4}' | tr , '\n' | egrep -v "$owner"
	fi
fi