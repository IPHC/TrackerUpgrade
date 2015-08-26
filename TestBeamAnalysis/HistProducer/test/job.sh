#!/bin/env sh

echo $1
echo $2
echo $3

inputFile="${1}"
outFile="${2}"
nmax=${3}

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:../.

../histProd ${inputFile} ${outFile} ${nmax}
