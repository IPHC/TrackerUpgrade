TrackerUpgrade
==============

Code related to the Test Beam data analysis for the CMS Tracker Phase-II Upgrade

The most updated code to use is TrackDUTanalyzer: performs directly matching of DUT analyzed Ntuples and EUDET software Ntuples.

cd TrackDUTanalyzer/src/

root -l

.L AutoDict_map_string_vector_int__.cxx++

.q

make

cd ../test

setenv LD_LIBRARY_PATH ${LD_LIBRARY_PATH}:/afs/cern.ch/work/c/chanon/Telescope/TrackerUpgrade/TrackDUTanalyzer/src

cmsStage /store/group/upgrade/Tracker/TestBeam/BTNov2015/DataAnalysisTree/USC.00000307.0001.A.storageManager.00.0000.root .

cmsStage /store/group/upgrade/Tracker/TestBeam/BTNov2015/TelescopeAnalysis/NtupleTelescope_918.root .

./runTrackDUTanalyzer USC.00000307.0001.A.storageManager.00.0000.root NtupleTelescope_918.root output.root 0 300000 --TrackMatchRun NovBT
