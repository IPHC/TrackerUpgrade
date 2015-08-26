#!/bin/env zsh

idir=/opt/sbg/cms/ui5_data1/kskovpen/trackerNEW/CMSSW_6_2_0_SLHC16/src/ILCSOFT/v01-17-05/CEDViewer/v01-07-02/
cd ${idir}
source build_env.sh
cd -

../ILCSOFT/v01-17-05/CEDViewer/v01-07-02/bin/ced2go \
-d ../ILCSOFT/v01-17-05/Eutelescope/trunk/jobsub/examples/cms-1FEI4-DUT/gear/gear_cern2015_FEI4.xml \
-t event-viewer-tmpl.xml \
output/lcio/run000347-track.slcio
