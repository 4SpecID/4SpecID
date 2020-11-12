#!/bin/sh

# Eduardo Conde-Sousa
# econdesousa@gmail.com
# v0.2

filename=$1
base=$(basename -- "$filename")
extension="${base##*.}"
dir=$(dirname -- "$filename")
base=$(echo $base | sed "s/\.$extension//")

head -1 $1 > ${dir}/.tmp
grep BOLD $1 >> ${dir}/.tmp
awk 'BEGIN{FS="\t";OFS="\t"}{if ($6=="Mined from GenBank, NCBI"){ for (i=1; i<6; i++) { printf $i "\t" }; system("./authors_from_GenBank_curl.sh " $71); printf "\t"; for (i=7; i<NF; i++) { printf $i "\t"}; print $NF }else{print}}' ${dir}/.tmp > ${dir}/${base}_BIN.${extension}

rm ${dir}/.tmp
