#!/bin/env zsh

rm -rf /tmp/kskovpen
mkdir /tmp/kskovpen

nmax=-1

finpath="/afs/cern.ch/work/k/kskovpen/public/tracker/TBNtuples/"
foutpath="hist/"

rm -rf ${foutpath}
mkdir ${foutpath}

# Full beam
inf=(
"${finpath}USC.00000815.0001.A.storageManager.00.0000.root" \
"${finpath}USC.00000781.0001.A.storageManager.00.0000.root" \
"${finpath}USC.00000782.0001.A.storageManager.00.0000.root" \
"${finpath}USC.00000783.0001.A.storageManager.00.0000.root" \
"${finpath}USC.00000774.0001.A.storageManager.00.0000.root" \
"${finpath}USC.00000788.0001.A.storageManager.00.0000.root" \
"${finpath}USC.00000780.0001.A.storageManager.00.0000.root" \
"${finpath}USC.00000786.0001.A.storageManager.00.0000.root" \
"${finpath}USC.00000787.0001.A.storageManager.00.0000.root" \
"${finpath}USC.00000789.0001.A.storageManager.00.0000.root" \
"${finpath}USC.00000775.0001.A.storageManager.00.0000.root" \
"${finpath}USC.00000776.0001.A.storageManager.00.0000.root" \
"${finpath}USC.00000779.0001.A.storageManager.00.0000.root")
outf=(
"deg0.root" \
"deg1.root" \
"deg2.root" \
"deg3.root" \
"deg5.root" \
"deg9.root" \
"deg10.root" \
"deg11.root" \
"deg12.root" \
"deg13.root" \
"deg15.root" \
"deg20.root" \
"deg30.root")

njobMax=20

rm -rf /tmp/kskovpen/*
rm -rf hist/*

for i in {1..${#inf[@]}}
do

list=($(pgrep histProd))
njobs=${#list[@]}
if [[ $njobs -ge ${njobMax} ]]; then
while true
do
sleep 1
list=($(pgrep histProd))
njobs=${#list[@]}
if [[ $njobs -lt ${njobMax} ]]; then
break
fi
done
fi

fin=${inf[$i]}
fout=${outf[$i]}
echo $fout
nohup ./job.sh ${fin} ${foutpath}${fout} ${nmax} > /tmp/kskovpen/${fout}.log 2>&1&
sleep 1

done
