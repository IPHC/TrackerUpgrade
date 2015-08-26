#!/bin/env zsh

finpath="/afs/cern.ch/work/k/kskovpen/public/tracker/TBNtuples/"
foutpath="hist/"

#nmax=10000
nmax=-1

rm -rf ${foutpath}
mkdir ${foutpath}

#inf=("${finpath}USC.00000816.0001.A.storageManager.00.0000.root" \
#"${finpath}USC.00000781.0001.A.storageManager.00.0000.root" \
#"${finpath}USC.00000782.0001.A.storageManager.00.0000.root" \
#"${finpath}USC.00000783.0001.A.storageManager.00.0000.root" \
#"${finpath}USC.00000774.0001.A.storageManager.00.0000.root" \
#"${finpath}USC.00000788.0001.A.storageManager.00.0000.root" \
#"${finpath}USC.00000780.0001.A.storageManager.00.0000.root" \
#"${finpath}USC.00000793.0001.A.storageManager.00.0000.root" \
#"${finpath}USC.00000775.0001.A.storageManager.00.0000.root" \
#"${finpath}USC.00000776.0001.A.storageManager.00.0000.root" \
#"${finpath}USC.00000777.0001.A.storageManager.00.0000.root")
#outf=("${foutpath}deg0.root" \
#"${foutpath}deg1.root" \
#"${foutpath}deg2.root" \
#"${foutpath}deg3.root" \
#"${foutpath}deg5.root" \
#"${foutpath}deg9.root" \
#"${foutpath}deg10.root" \
#"${foutpath}deg11.root" \
#"${foutpath}deg15.root" \
#"${foutpath}deg20.root" \
#"${foutpath}deg25.root")

inf=("${finpath}USC.00000762.0001.A.storageManager.00.0000.root" \
"${finpath}USC.00000773.0001.A.storageManager.00.0000.root" \
"${finpath}USC.00000768.0001.A.storageManager.00.0000.root" \
"${finpath}USC.00000831.0001.A.storageManager.00.0000.root" \
"${finpath}USC.00000844.0001.A.storageManager.00.0000.root" \
"${finpath}USC.00000816.0001.A.storageManager.00.0000.root")
outf=("${foutpath}762.root" \
"${foutpath}773.root" \
"${foutpath}768.root" \
"${foutpath}831.root" \
"${foutpath}844.root" \
"${foutpath}816.root")

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:../.

for i in {1..${#inf[@]}}
do
echo "Processing ${inf[$i]} (${outf[$i]}) ${nmax}"
../histProd ${inf[$i]} ${outf[$i]} ${nmax}
done
