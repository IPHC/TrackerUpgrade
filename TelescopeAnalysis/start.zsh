#!/bin/env zsh

idir=/opt/sbg/cms/ui5_data1/kskovpen/trackerNEW/CMSSW_6_2_0_SLHC16/src/ILCSOFT/v01-17-05/Eutelescope/trunk
cd ${idir}
source build_env.sh
cd -

export ANALYSIS=/opt/sbg/cms/ui5_data1/kskovpen/trackerNEW/CMSSW_6_2_0_SLHC16/src/TelescopeAnalysis
export ANALYSIS_CONF=$EUTELESCOPE/jobsub/examples/cms-1FEI4-DUT
