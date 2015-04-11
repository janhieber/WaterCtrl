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

		# this is google search
		#xdg-open "https://www.google.de/#hl=de&tbs=lr:lang_1de&lr=lang_de&q=site:www.reichelt.de+%22${art}%22"

		# this is dirty but opens the item directly
		URL=$(curl -X POST -H 'Content-Type: application/x-www-form-urlencoded' -d "SEARCH=${art}&dummy=" http://www.reichelt.de/index.html\?\&ACTION\=446\&LA\=3 2>/dev/null | grep '<a class="al_artinfo_link" itemprop="url"' | head -n 1 | awk '{ print $4 }' | sed "s/href=//g" | sed 's/"//g')
		firefox $URL
		read -n1 -p "Press ENTER to order next ..."
	fi
done 10<BOM.csv
