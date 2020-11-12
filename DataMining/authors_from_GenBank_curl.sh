#!/bin/sh

# Eduardo Conde-Sousa
# econdesousa@gmail.com
# v0.1


 
curl -s  "https://eutils.ncbi.nlm.nih.gov/entrez/eutils/efetch.fcgi?db=nucleotide&id=${1}&rettype=gb&retmode=txt" | grep AUTHORS   | head -1 | awk '{for (i=2; i<=NF; i++) printf $i " "; }'



