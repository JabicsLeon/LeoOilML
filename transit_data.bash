#!/bin/bash

if [[ -n "$1" ]]; then
	CDIR="$1"
else
	CDIR=".."
fi

if [[ -n "$2" ]]; then
	DIR="$2"
	
else
	DIR=$(pwd)
fi

#Set way to dir
DIR_NAME=$(basename "$DIR")

cd "$DIR"

#Check and create dir
mkdir -p LAS DLIS CSV XLSX

#Set ways to data dirs
las="$DIR/LAS"
dlis="$DIR/DLIS"
csv="$DIR/CSV"
xlsx="$DIR/XLSX"

cd "$CDIR"

find . -path "./$DIR_NAME" -prune -o -type f -name "*.las" -exec mv {} "$las" \;
find . -path "./$DIR_NAME" -prune -o -type f -name "*.LAS" -exec mv {} "$las" \;
find . -path "./$DIR_NAME" -prune -o -type f -name "*.dlis" -exec mv {} "$dlis" \;
find . -path "./$DIR_NAME" -prune -o -type f -name "*.DLIS" -exec mv {} "$dlis" \;
find . -path "./$DIR_NAME" -prune -o -type f -name "*.csv" -exec mv {} "$csv" \;
find . -path "./$DIR_NAME" -prune -o -type f -name "*.xlsx" -exec mv {} "$xlsx" \;

