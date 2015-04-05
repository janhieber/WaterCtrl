#!/bin/sh

frstLn=true

while read -u 10 line
do
	if $frstLn
	then
		frstLn=false
		echo "skip"
	else
		line1=$(echo $line | sed 's/"//g')
		qty=$(echo $line1 | awk -F ';' '{ print $1 }')
		art=$(echo $line1 | awk -F ';' '{ print $11 }')
		echo -e "\npart: ${art} \t\tQTY: ${qty}"
		xdg-open "https://www.google.de/#hl=de&tbs=lr:lang_1de&lr=lang_de&q=site:www.reichelt.de+%22${art}%22"
		read -n1 -p "Press ENTER to order next ..."
	fi
done 10<BOM.csv
