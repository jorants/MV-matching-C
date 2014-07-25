#!/bin/bash

# This script copies the NodeList struct and related functions
# and converts them into PointerList and EdgeList structs,
# which are basically identical structures, with different contents

# I.e., this is a poor-man's c++ template

trap "exit" INT

rm mv_progress/*  &> /dev/null
touch mv_progress/.touch

./mvmatching $@


# rm out.pdf

for f in mv_progress/*.dot
do
	dot -Tpdf -o$f.pdf $f &> /dev/null
done

PDFS=mv_progress/*.pdf

pdfunite $PDFS mv_progress/out.pdf  &> /dev/null

for f in mv_progress/*.dot
do
	rm $f.pdf  &> /dev/null
done

