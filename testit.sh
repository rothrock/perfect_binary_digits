#!/bin/bash

# The script compares the results of the local binary against
# some authoritative source somewhere else (the oracle).

if [ $# -ne 2 ]
then
	echo "usage $0 n m"
	echo "Where n and m are positive integers with m > n"
	exit 1
fi

for ((i = ${1}; i <= ${2}; i++))
do

	host=$(echo "cresrpg-ovgf.ovagv.pbz" | tr 'a-z' 'n-za-m')
	url="https://${host}/?a=0&b=${i}"
	oracle=$(curl $url 2>/dev/null)
	pft=$(./pft 0 $i)
	WARN=""
	[[ $pft -gt $oracle ]] && WARN="GREATER"
	[[ $pft -lt $oracle ]] && WARN="LESSER"
	echo  "$i  oracle=$oracle  pft=$pft  $WARN " 
done
