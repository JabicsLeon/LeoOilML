#!/bin/bash

#Set way to dir
DIR=$(pwd)
DIR_NAME=$(basename "$DIR")

#Check and create dir
mkdir -p LAS DLIS CSV XLSX

#Set ways to data dirs
las="$DIR/LAS"
dlis="$DIR/DLIS"
csv="$DIR/CSV"
xlsx="$DIR/XLSX"

cd ..

find . -path "./$DIR_NAME" -prune -o -type f -name "*.las" -exec cp {} "$las" \;
find . -path "./$DIR_NAME" -prune -o -type f -name "*.LAS" -exec cp {} "$las" \;
find . -path "./$DIR_NAME" -prune -o -type f -name "*.dlis" -exec cp {} "$dlis" \;
find . -path "./$DIR_NAME" -prune -o -type f -name "*.DLIS" -exec cp {} "$dlis" \;
find . -path "./$DIR_NAME" -prune -o -type f -name "*.csv" -exec cp {} "$csv" \;
find . -path "./$DIR_NAME" -prune -o -type f -name "*.xlsx" -exec cp {} "$xlsx" \;

cd "$DIR"
