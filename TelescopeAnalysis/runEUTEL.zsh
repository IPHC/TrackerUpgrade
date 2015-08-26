#!/bin/env zsh

runNumber=339

# converter
$EUTELESCOPE/jobsub/jobsub.py --config=${ANALYSIS_CONF}/config.cfg \
-csv ${ANALYSIS_CONF}/runlist.csv converter ${runNumber} -g

# clustering
$EUTELESCOPE/jobsub/jobsub.py --config=${ANALYSIS_CONF}/config.cfg \
-csv ${ANALYSIS_CONF}/runlist.csv clustering ${runNumber} -g

# hitmaker
$EUTELESCOPE/jobsub/jobsub.py --config=${ANALYSIS_CONF}/config.cfg \
-csv ${ANALYSIS_CONF}/runlist.csv hitmaker ${runNumber} -g

# align
$EUTELESCOPE/jobsub/jobsub.py --config=${ANALYSIS_CONF}/config.cfg \
-csv ${ANALYSIS_CONF}/runlist.csv align ${runNumber} -g

# fitter
$EUTELESCOPE/jobsub/jobsub.py --config=${ANALYSIS_CONF}/config.cfg \
-csv ${ANALYSIS_CONF}/runlist.csv fitter ${runNumber} -g
