#!/bin/bash

while read line
do

  echo $line > tmp
  tel=`awk '{print $1}' tmp`
  dut=`awk '{print $2}' tmp`
  echo $tel $dut

./runTrackDUTanalyzer /afs/cern.ch/work/d/delcourt/private/tb201511/ntuple/USC.00000${dut}.0001.A.storageManager.00.0000.root /afs/cern.ch/work/c/chanon/Telescope/data/NtupleTelescope_${tel}.root output.root 0 1000000 --TrackMatchRun NovBT

mv output.root output_${tel}.root

done < runlistBatch.txt

