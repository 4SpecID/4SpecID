#!/bin/sh

# Eduardo Conde-Sousa
# econdesousa@gmail.com
# v0.2

taxa=$1
if [ $# -lt 2 ]; then 
    outfolder="../"
else
	outfolder=$2
fi


echo $taxa
echo $outfolder
fName=$(echo $taxa | sed 's/ /_/g')

echo -e "Downloading " $taxa " to " ${outfolder}/${fName}.tsv
wget -O ${outfolder}/${fName}.tsv "http://v4.boldsystems.org/index.php/API_Public/combined?taxon=${taxa}&format=tsv"


echo "Searching for linked data"
for BIN in $(awk 'BEGIN{FS="\t"}{if ($8 !="bin_uri"){print $8}}' ${outfolder}/${fName}.tsv |sort | uniq); do
	wget -O ${outfolder}/${BIN}.tsv "http://v4.boldsystems.org/index.php/API_Public/combined?bin=${BIN}&format=tsv"
	cat ${outfolder}/${fName}.tsv > ${outfolder}/.tmp
	tail -n +2 ${outfolder}/${BIN}.tsv >> ${outfolder}/.tmp 
	mv ${outfolder}/.tmp ${outfolder}/${fName}.tsv 
	rm ${outfolder}/${BIN}.tsv
done


echo "search for unique rows"
awk -F, '!seen[$0]++' ${outfolder}/${fName}.tsv > ${outfolder}/${fName}_uniq.tsv
mv ${outfolder}/${fName}_uniq.tsv ${outfolder}/${fName}.tsv

echo "searching for GenBank data sources... this may take a while"
./filter_by_BIN_and_add_GenBank_info.sh ${outfolder}/${fName}.tsv

echo "Done!"
