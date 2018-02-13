#!/bin/bash

for ((i=${1}; i<${2}; i++))
do
	url="https://perfect-binary-digits.some-oracle.com/?a=0&b=$i"
	oracle=$(curl $url 2>/dev/null)
	pft=$(./pft 0 $i)
	WARN=""
	[[ $pft -gt $oracle ]] && WARN="GREATER"
	[[ $pft -lt $oracle ]] && WARN="LESSER"
	echo  "$i  oracle=$oracle  pft=$pft  $WARN " 
done
