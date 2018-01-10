#!/bin/bash

for ((i=${1}; i<${2}; i++))
do
	url="https://perfect-bits.binti.com/?a=0&b=$i"
	binti=$(curl $url 2>/dev/null)
	pft=$(./pft 0 $i)
	WARN=""
	[[ $pft -gt $binti ]] && WARN="GREATER"
	[[ $pft -lt $binti ]] && WARN="LESSER"
	echo  "$i  binti=$binti  pft=$pft  $WARN " 
done
